
#include "stdafx.h"

#include "MyCallback3D.h"
#include "MyCanvas3D.h"
#include "MyInteractionStyle3D.h"
#include "Reslicer.h"
#include "ImageDetailsType.h"


int main(int argc, char *argv[])
{
	auto input_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	input_reader->SetDirectoryName("E:\\misSoloution\\CreateMISRelease_local\\Images\\brain_tumor_2");
	input_reader->Update();

	//---------------------------------------visualization-----------------------------

	Reslicer* IS_Reslicer = new Reslicer(input_reader);
	IS_Reslicer->PrepareInputImage(input_reader);

	auto ResliceDicom3D = vtkSmartPointer<vtkImageReslice>::New();
	IS_Reslicer->SetReslice3D(ResliceDicom3D);
	
	auto DataActor = vtkSmartPointer<vtkImageActor>::New();
	MyCanvas3D* IS_MyCanvas3D = new MyCanvas3D(ResliceDicom3D);
	IS_MyCanvas3D->SetImageData(ResliceDicom3D->GetOutput());
	IS_Reslicer->SetCanvas(IS_MyCanvas3D);

	auto ResliceDicom2D = vtkSmartPointer<vtkImageReslice>::New();
	IS_Reslicer->SetReslice2D(ResliceDicom2D);
	ResliceDicom2D->Update();

	DataActor->GetMapper()->SetInputConnection(ResliceDicom2D->GetOutputPort());

	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(DataActor);
	renderer->GetActiveCamera()->ParallelProjectionOn();
	renderer->ResetCamera();
	IS_MyCanvas3D->SetRenderer(renderer);

	auto window = vtkSmartPointer<vtkRenderWindow>::New();
	window->AddRenderer(renderer);
	IS_MyCanvas3D->SetWindow(window);

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<MyInteractionStyle3D> imageStyle = new MyInteractionStyle3D;
	IS_MyCanvas3D->SetStyle(imageStyle);
	IS_MyCanvas3D->SetInteractor(interactor);
	interactor->SetInteractorStyle(imageStyle);
	window->SetInteractor(interactor);
	imageStyle->SetCurrentRenderer(renderer);
	window->Render();

	//------------------------------------------Algorithm-------------------------------------------
	
	auto IS_Callback = vtkSmartPointer<MyCallback3D>::New();
	IS_Callback->SetImageReslice(ResliceDicom3D, ResliceDicom2D); // "ResliceDicom2D" For overlay
	IS_Callback->SetRenderer(renderer); //For overlay
	IS_Callback->SetWindow(window); //For overlay
	IS_Callback->SetInteractor(interactor);
	IS_Callback->SetDiagram(IS_MyCanvas3D);
	IS_Callback->SetStyle(imageStyle);

	imageStyle->AddObserver(vtkCommand::MouseWheelForwardEvent, IS_Callback);
	imageStyle->AddObserver(vtkCommand::MouseWheelBackwardEvent, IS_Callback);
	interactor->AddObserver(vtkCommand::LeftButtonPressEvent, IS_Callback);
	interactor->Start();

	//------------------------------------------ Write Final ---------------------------------------

	typedef itk::GDCMImageIO    ImageIOType;
	ImageIOType::Pointer myOutput = ImageIOType::New();
	itk::MetaDataDictionary & dict = myOutput->GetMetaDataDictionary();
	std::string tagkey, value;
	tagkey = "0008|0060"; // Modality
	value = "MR";
	itk::EncapsulateMetaData<std::string>(dict, tagkey, value);
	tagkey = "0008|0008"; // Image Type
	value = "DERIVED\\SECONDARY";
	itk::EncapsulateMetaData<std::string>(dict, tagkey, value);
	tagkey = "0008|0064"; // Conversion Type
	value = "DV";
	itk::EncapsulateMetaData<std::string>(dict, tagkey, value);
	myOutput->SetSpacing(2, IS_Reslicer->GetSpacing()[2]);

	typedef itk::Image< OutputPixelType, 2 >  Image2DType;
	typedef itk::ImageSeriesWriter<OutputImageType, Image2DType >  SeriesWriterType;
	SeriesWriterType::Pointer seriesResult = SeriesWriterType::New();
	seriesResult->SetInput(IS_Callback->GetFinalResult()->GetOutput());
	seriesResult->SetImageIO(myOutput);

	OutputImageType::RegionType region =
		IS_Callback->GetInternalImage()->GetLargestPossibleRegion();
	OutputImageType::IndexType start = region.GetIndex();
	OutputImageType::SizeType  size = region.GetSize();

	const char * outputDirectory = "E:\\Interactive_Segmentation\\output3D\\";
	std::string format = outputDirectory;
	format += "/image%03d.dcm";

	typedef itk::NumericSeriesFileNames   NamesGeneratorType;
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

	namesGenerator->SetSeriesFormat(format.c_str());
	namesGenerator->SetStartIndex(start[2]);
	namesGenerator->SetEndIndex(start[2] + size[2] - 1);
	namesGenerator->SetIncrementIndex(1);

	seriesResult->SetFileNames(namesGenerator->GetFileNames());

	try
	{
		seriesResult->Update();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown while writing the series " << std::endl;
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}