#include "stdafx.h"
#include "Algorithm.h"

Algorithm::Algorithm()
{}

void Algorithm::set_Canvas(Canvas2D* Diagram)
{
	diagram = Diagram;
}

void Algorithm::set_reader(itk::SmartPointer<InternalImageType > _reader)
{
	reader = _reader;
}

void Algorithm::Set_Function(itk::SmartPointer<MySpeedType> _Function)
{
	my_function = _Function;
}


void Algorithm::FastMarching(const double distance)
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
	fastMarching->SetOutputRegion(reader->GetBufferedRegion());
	fastMarching->SetOutputSpacing(reader->GetSpacing());
	fastMarching->SetOutputOrigin(reader->GetOrigin());
	fastMarching->SetOutputDirection(reader->GetDirection());

}

void Algorithm::Level_Set(int lower, int upper, double edge, double weight)
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
	thresholdSegmentation->SetFeatureImage(reader);
	thresholder->SetInput(thresholdSegmentation->GetOutput());
	//thresholder->Update();

	InternalImageType_2_OutputImageType::Pointer cast_outout = InternalImageType_2_OutputImageType::New();
	cast_outout->SetInput(fastMarching->GetOutput());
	cast_outout->Update();


}

void Algorithm::Level_Set(double edge, double weight)
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
	thresholdSegmentation->SetFeatureImage(reader);
	thresholder->SetInput(thresholdSegmentation->GetOutput());
	//thresholder->Update();

	InternalImageType_2_OutputImageType::Pointer cast_outout = InternalImageType_2_OutputImageType::New();
	cast_outout->SetInput(fastMarching->GetOutput());
	cast_outout->Update();
}


int Algorithm::WriteImage(std::string output1)
{
	writer1->SetInput(thresholder->GetOutput());
	writer1->SetFileName(output1.c_str());
	
	try
	{
		writer1->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
		return EXIT_FAILURE;
	}

	//std::cout << std::endl;
	//std::cout << "Max. no. iterations: " << thresholdSegmentation->GetNumberOfIterations() << std::endl;
	//std::cout << "Max. RMS error: " << thresholdSegmentation->GetMaximumRMSError() << std::endl;
	//std::cout << std::endl;
	//std::cout << "No. elpased iterations: " << thresholdSegmentation->GetElapsedIterations() << std::endl;
	//std::cout << "RMS change: " << thresholdSegmentation->GetRMSChange() << std::endl;

	//InternalWriterType::Pointer mapWriter = InternalWriterType::New();
	//mapWriter->SetInput(fastMarching->GetOutput());
	//mapWriter->SetFileName("fastMarchingImage.mha");
	//mapWriter->Update();

	//InternalWriterType::Pointer speedWriter = InternalWriterType::New();
	//speedWriter->SetInput(thresholdSegmentation->GetSpeedImage());
	//speedWriter->SetFileName("speedTermImage.mha");
	//speedWriter->Update();

	return EXIT_SUCCESS;
}


InternalImageType * Algorithm::Get_FastMarching()
{
	return fastMarching->GetOutput();
}

OutputImageType * Algorithm::Get_thresholder()
{
	return thresholder->GetOutput();
}
