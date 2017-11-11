#ifndef _POINT_CLOUD_FIT_H_FILE_
#define _POINT_CLOUD_FIT_H_FILE_

#include "MeshDoc.h"
#include "Satellite.h"

void doFailure();

#define _MySatePtAttri "PerVerAttri_SatePt"
typedef int SatePtType;
#define MaxOlaneNum 256
enum _SatePtType {
	Pt_Undefined = 0x000,
	Pt_Noise = 0x100,
	Pt_OnMBCylinder = 0x800,
	Pt_OnMBCube = 0x600,
	Pt_OnPlane = 0x0200
};


class PCFit
{
public:
	PCFit(const int nThread = 0);
	PCFit(const char *PlyFilePath, const int nThread = 0);
	~PCFit();

	void printLogo();
	void printParams();

	void setThreadNum(const int nThread);
	void initMParams(const char *iniFile = 0);

	bool loadPly(const char *PlyFilePath);
	bool savePly(const char *PlyFilePath);
	void clearD();
	void inverseD();
	int deletePts(const int mask);
	int keepPts(const int mask);
	int recolorPts(const int mask, const unsigned char R, const unsigned char G, const unsigned char B, const unsigned char A = 255);
	void autoColor();

	bool Fit_Sate(bool keepAttribute = true);
	Satellite *getSate() { return m_sate; }
private:
	MeshDocument m_meshDoc;
	Satellite *m_sate;
	double m_refa;

	// All Thresholds
	int Threshold_NPts;                      // 整体点云数小于[Threshold_NPts]不进行处理
	// double RefA_Ratio;                       // 单位长度比例
	int PlaneNum_Expected;                   // 期望平面个数

	int DeNoise_MaxIteration;                // 去噪迭代阈值, 默认100(<=0)
	int DeNoise_KNNNeighbors;                // 去噪时KNN最近邻个数(KNN)
    int DeNoise_GrowNeighbors;               // 去噪时KNN最近邻个数(区域生长)
	double DeNoise_DisRatioOfOutlier;        // 去噪距离比例阈值

	double Precision_HT;                     // HT系数
	double Threshold_NPtsPlane;              // 平面点个数比例阈值
	double Threshold_DisToPlane;             // 平面检测距离阈值系数
	double Threshold_AngToPlane;             // 平面检测角度阈值

	double Threshold_PRAng;                  // 平面关系角度阈值
	double Threshold_PRDis;                  // 平面关系距离阈值系数
	double Threshold_NPtsCyl;                // 圆柱点个数阈值系数
private:
	// Preproc
	int DeNoiseKNN();
	int DeNoiseRegGrw();
	double GetMeshSizeAlongN(const vcg::Point3f n);
	bool PCADimensionAna(vcg::Point3f &PSize, std::vector<vcg::Point3f> &PDirections, bool leftNoisePts);
	double RoughnessAna(bool leftNoisePts = true);

    vcg::Point3f GetPointList(
        std::vector<int> &indexList,
        std::vector<vcg::Point3f> &pointList,
        std::vector<vcg::Point3f> &normList,
        const bool bNormalize = false);

	// Detect Plane
	std::vector<Sailboard*> DetectPlanesHT(const int expPN);
    std::vector<Sailboard*> DetectPlanesGCO(const int expPN);
    std::vector<Sailboard*> DetectPlanesGCO_HT(
        const int expPN, 
        const int numNeighbors,
        const int iteration = -1);
	
	// Refer Cude
	std::vector<Sailboard*> CuboidFaceInferring(const std::vector<Sailboard*> &planes);
	MainBodyCube* CuboidMeasure(const std::vector<Sailboard*> &CubePlanes);
	MainBodyCube* DetectCuboidFromPlanes(std::vector<Sailboard*> &planes);
	
	// Detect Cylinder
	MainBodyCylinder* DetectCylinder();    
};

#endif // !_POINT_CLOUD_FIT_H_FILE_
