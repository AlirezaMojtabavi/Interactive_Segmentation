

#include "stdafx.h"
#include "MyAlgorithm3d.h"


MyAlgorithm3d::MyAlgorithm3d()
{
}

void MyAlgorithm3d::SetInternalImage(InternalImageType::Pointer _InternalImage)
{
	IS_InternalImage = _InternalImage;
}

void MyAlgorithm3d::SetCanvas(MyCanvas3D* _IS_MyCanvas3D)
{
	IS_MyCanvas3D = _IS_MyCanvas3D;
}

void MyAlgorithm3d::SetStyle(MyInteractionStyle3D * _style)
{
	style = _style;
}

InternalImageType * MyAlgorithm3d::GetFastMarching()
{
	return fastMarching->GetOutput();
}

OutputImageType * MyAlgorithm3d::GetThresholder()
{
	return thresholder->GetOutput();
}


void MyAlgorithm3d::SetSpeedFunction(itk::SmartPointer<MySpeedFunction3DType> _Function)
{
	SegmentationSpeedFunction = _Function;
}

void MyAlgorithm3d::FastMarching(const double distance)
{
	int size = IS_MyCanvas3D->GetVectorOfPoints().size();

	std::vector<InternalImageType::IndexType>  seedPosition(size);
	std::vector<NodeType> node(size);

	const double initialDistance = distance;
	const double seedValue = -initialDistance;

	seeds->Initialize();

	for (int i = 0; i < size; i++)
	{
		seedPosition[i][0] = IS_MyCanvas3D->GetVectorOfPoints()[i]._x;
		seedPosition[i][1] = IS_MyCanvas3D->GetVectorOfPoints()[i]._y;
		seedPosition[i][2] = IS_MyCanvas3D->GetVectorOfPoints()[i]._z;

		node[i].SetValue(seedValue);
		node[i].SetIndex(seedPosition[i]);

		seeds->InsertElement(i, node[i]);
	}

	cout << "\n\n";

	fastMarching->SetTrialPoints(seeds);
	fastMarching->SetSpeedConstant(1.0);

	IS_InternalImage->Update();

	fastMarching->SetOutputRegion(IS_InternalImage->GetBufferedRegion());
	fastMarching->SetOutputSpacing(IS_InternalImage->GetSpacing());
	fastMarching->SetOutputOrigin(IS_InternalImage->GetOrigin());
	fastMarching->SetOutputDirection(IS_InternalImage->GetDirection());
}

void MyAlgorithm3d::LevelSet(short int lower, short int upper, double edge, double weight)
{
	thresholder->SetLowerThreshold(-1000);
	thresholder->SetUpperThreshold(0);

	thresholder->SetOutsideValue(0.0);
	thresholder->SetInsideValue(255.0);

	SegmentationSpeedFunction->SetLowerThreshold(lower);
	SegmentationSpeedFunction->SetUpperThreshold(upper);
	SegmentationSpeedFunction->SetEdgeWeight(edge);
	SegmentationSpeedFunction->SetFeatureImage(IS_InternalImage);

	double curvature = (lower + upper) / 2;
	thresholdSegmentation->SetSegmentationFunction(SegmentationSpeedFunction);
	thresholdSegmentation->SetPropagationScaling(1.0);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(IS_InternalImage);

	thresholder->SetInput(thresholdSegmentation->GetOutput());

	//thresholder->Update();
}

void MyAlgorithm3d::LevelSet(double edge, double weight)
{
	auto window = IS_MyCanvas3D->GetMinMaxIntensity();
	double lower = window[0];
	double upper = window[1];

	cout << "lower : " << lower << "\t\t" << "upper : " << upper << endl;

	thresholder->SetLowerThreshold(-1000);
	thresholder->SetUpperThreshold(0);

	thresholder->SetOutsideValue(0.0);
	thresholder->SetInsideValue(255.0);

	SegmentationSpeedFunction->SetLowerThreshold(lower);
	SegmentationSpeedFunction->SetUpperThreshold(upper);
	SegmentationSpeedFunction->SetEdgeWeight(edge);
	SegmentationSpeedFunction->SetFeatureImage(IS_InternalImage);

	double curvature = (lower + upper) / 2;
	thresholdSegmentation->SetSegmentationFunction(SegmentationSpeedFunction);
	thresholdSegmentation->SetPropagationScaling(1.0);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(IS_InternalImage);
	thresholder->SetInput(thresholdSegmentation->GetOutput());
	//thresholder->Update();
}
