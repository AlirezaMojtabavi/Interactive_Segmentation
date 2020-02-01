#include "stdafx.h"
#include "Algorithm2D.h"

Algorithm2D::Algorithm2D()
{}

void Algorithm2D::set_Canvas(Canvas2D* Diagram)
{
	diagram = Diagram;
}

void Algorithm2D::set_reader(itk::SmartPointer<ImageType_2_InternalImageType> _reader)
{
	reader = _reader;
}

void Algorithm2D::Set_Function(itk::SmartPointer<MySpeedFunction2DType> _Function)
{
	my_function = _Function;
}

void Algorithm2D::FastMarching(const double distance)
{
	int size = diagram->get_vector().size();

	std::vector<InternalImageType::IndexType>  seedPosition(size);
	std::vector<NodeType> node(size);

	const double initialDistance = distance;
	const double seedValue = -initialDistance;

	seeds->Initialize();

	for (int i = 0; i < size; i++)
	{
		seedPosition[i][0] = diagram->get_vector()[i]._x;
		seedPosition[i][1] = diagram->get_vector()[i]._y;

		cout << seedPosition[i] << "\t";

		node[i].SetValue(seedValue);
		node[i].SetIndex(seedPosition[i]);

		seeds->InsertElement(i, node[i]);
	}

	fastMarching->SetTrialPoints(seeds);
	fastMarching->SetSpeedConstant(1.0);

	reader->Update();
	const InternalImageType * inputImage = reader->GetOutput();
	fastMarching->SetOutputRegion(inputImage->GetBufferedRegion());
	fastMarching->SetOutputSpacing(inputImage->GetSpacing());
	fastMarching->SetOutputOrigin(inputImage->GetOrigin());
	fastMarching->SetOutputDirection(inputImage->GetDirection());

}

void Algorithm2D::Level_Set(int lower, int upper, double edge, double weight)
{
	thresholder->SetLowerThreshold(-1000);
	thresholder->SetUpperThreshold(0);

	thresholder->SetOutsideValue(0.0);
	thresholder->SetInsideValue(255.0);

	double curvature = (lower + upper) / 2;
	thresholdSegmentation->SetSegmentationFunction(my_function);
	thresholdSegmentation->SetPropagationScaling(1.0);

	my_function->SetLowerThreshold(lower);
	my_function->SetUpperThreshold(upper);
	my_function->SetEdgeWeight(edge);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(reader->GetOutput());
	thresholder->SetInput(thresholdSegmentation->GetOutput());
	//thresholder->Update();
}

void Algorithm2D::Level_Set(double edge, double weight)
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
	thresholdSegmentation->SetSegmentationFunction(my_function);
	thresholdSegmentation->SetPropagationScaling(1.0);

	my_function->SetLowerThreshold(lower);
	my_function->SetUpperThreshold(upper);
	my_function->SetEdgeWeight(edge);

	thresholdSegmentation->SetCurvatureScaling(weight * curvature);

	thresholdSegmentation->SetMaximumRMSError(0.01);
	thresholdSegmentation->SetNumberOfIterations(300);
	thresholdSegmentation->SetIsoSurfaceValue(0.0);

	thresholdSegmentation->SetInput(fastMarching->GetOutput());
	thresholdSegmentation->SetFeatureImage(reader->GetOutput());
	thresholder->SetInput(thresholdSegmentation->GetOutput());
	//thresholder->Update();
}

InternalImageType * Algorithm2D::Get_FastMarching()
{
	return fastMarching->GetOutput();
}

OutputImageType * Algorithm2D::Get_thresholder()
{
	return thresholder->GetOutput();
}
