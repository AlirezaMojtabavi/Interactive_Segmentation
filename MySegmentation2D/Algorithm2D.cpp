#include "stdafx.h"
#include "Algorithm2D.h"

Algorithm2D::Algorithm2D()
{}

void Algorithm2D::SetInternalImage(itk::SmartPointer<ImageType_2_InternalImageType> _IS2D_InternalImage)
{
	IS2D_InternalImage = _IS2D_InternalImage;
}

void Algorithm2D::SetCanvas(Canvas2D* Diagram)
{
	diagram = Diagram;
}

void Algorithm2D::SetSpeedFunction(itk::SmartPointer<MySpeedFunction2DType> _Function)
{
	mySpeedFunction = _Function;
}

void Algorithm2D::FastMarching(const double distance)
{
	int size = diagram->GetPointList().size();

	std::vector<InternalImageType::IndexType>  seedPosition(size);
	std::vector<NodeType> node(size);

	const double initialDistance = distance;
	const double seedValue = -initialDistance;

	seeds->Initialize();

	for (int i = 0; i < size; i++)
	{
		seedPosition[i][0] = diagram->GetPointList()[i]._x;
		seedPosition[i][1] = diagram->GetPointList()[i]._y;
		cout << seedPosition[i] << "\n";

		node[i].SetValue(seedValue);
		node[i].SetIndex(seedPosition[i]);

		seeds->InsertElement(i, node[i]);
	}
	fastMarching->SetTrialPoints(seeds);
	fastMarching->SetSpeedConstant(1.0);
	IS2D_InternalImage->Update();

	fastMarching->SetOutputRegion(IS2D_InternalImage->GetOutput()->GetBufferedRegion());
	fastMarching->SetOutputSpacing(IS2D_InternalImage->GetOutput()->GetSpacing());
	fastMarching->SetOutputOrigin(IS2D_InternalImage->GetOutput()->GetOrigin());
	fastMarching->SetOutputDirection(IS2D_InternalImage->GetOutput()->GetDirection());
}

void Algorithm2D::LevelSet(int lower, int upper, double edge, double weight)
{
	thresholder->SetLowerThreshold(-1000);
	thresholder->SetUpperThreshold(0);

	thresholder->SetOutsideValue(0.0);
	thresholder->SetInsideValue(255.0);

	double curvature = (lower + upper) / 2;
	thresholdSegmentation->SetSegmentationFunction(mySpeedFunction);
	thresholdSegmentation->SetPropagationScaling(1.0);

	mySpeedFunction->SetLowerThreshold(lower);
	mySpeedFunction->SetUpperThreshold(upper);
	mySpeedFunction->SetEdgeWeight(edge);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(IS2D_InternalImage->GetOutput());
	thresholder->SetInput(thresholdSegmentation->GetOutput());
}

void Algorithm2D::LevelSet(double edge, double weight) // without setting lower and upper threshold
{
	auto window = diagram->get_min_max();
	double lower = window[0];
	double upper = window[1];

	cout << "lower : " << lower << "\t\t" << "upper : " << upper << endl;

	thresholder->SetLowerThreshold(-1000);
	thresholder->SetUpperThreshold(0);

	thresholder->SetOutsideValue(0.0);
	thresholder->SetInsideValue(255.0);

	double curvature = (lower + upper) / 2;
	thresholdSegmentation->SetSegmentationFunction(mySpeedFunction);
	thresholdSegmentation->SetPropagationScaling(1.0);

	mySpeedFunction->SetLowerThreshold(lower);
	mySpeedFunction->SetUpperThreshold(upper);
	mySpeedFunction->SetEdgeWeight(edge);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(IS2D_InternalImage->GetOutput());
	thresholder->SetInput(thresholdSegmentation->GetOutput());
}

OutputImageType * Algorithm2D::GetResult()
{
	return thresholder->GetOutput();
}
