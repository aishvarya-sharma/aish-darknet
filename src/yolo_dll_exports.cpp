#include "yolo_v2_class.hpp"


extern "C" {
	YOLODLL_API void initDetector(const char *cfg, int cfgSize, const char *weights, int weightsSize);
	YOLODLL_API void closeDetector();
	YOLODLL_API void detectFrame(unsigned int width, unsigned int height, unsigned char *imgData, float thresh,
		intptr_t *hItems, bbox_t **itemsFound, int *itemCount);
}

static Detector *g_Instance = NULL;
YOLODLL_API void  initDetector(const char *cfg, int cfgSize, const char *weights, int weightsSize) {
	std::string cfgPath(cfg, cfgSize);
	std::string weightsPath(weights, weightsSize);
	g_Instance = new Detector(cfgPath, weightsPath);
};

YOLODLL_API void   closeDetector()
{
	delete g_Instance;
	g_Instance = NULL;
};

YOLODLL_API int detectFrame(const char *strFileName, int strFileNameSize, float thresh, intptr_t *hItems, bbox_t **itemsFound, int *itemCount)
{
	if (g_Instance == NULL)
	{
		return -1;			// Not initialized
	}
	std::string fileName(strFileName, strFileNameSize);
	std::vector<bbox_t> detectOut = g_Instance->detect(fileName, thresh);
	*hItems = reinterpret_cast<intptr_t>(&detectOut);
	*itemsFound = detectOut.data();
	*itemCount = detectOut.size();

	return 0;
}
