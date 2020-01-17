
#include "stdafx.h"

#include "MySpeedFunction3D.h"
#include "MyCallback3D.h"
#include "MyCanvas3D.h"
#include "MyInteractionStyle3D.h"
#include"Reslicer.h"


int main(int argc, char *argv[])
{
	VTK_CREATE(vtkDICOMImageReader, input_reader);
	input_reader->SetDirectoryName("E:\\Interactive_Segmentation\\input3D");
	input_reader->Update();

	//---------------------------------------visualization-----------------------------

	Reslicer* IS_Reslicer = new Reslicer(input_reader);
	IS_Reslicer->PrepareInputImage(input_reader);

	VTK_CREATE(vtkImageReslice, ResliceDicom3D);
	IS_Reslicer->SetReslice3D(ResliceDicom3D);
	
	VTK_CREATE(vtkImageActor, DataActor);
	MyCanvas3D* IS_MyCanvas3D = new MyCanvas3D(ResliceDicom3D);
	IS_MyCanvas3D->SetImageData(ResliceDicom3D->GetOutput());
	IS_Reslicer->SetCanvas(IS_MyCanvas3D);

	VTK_CREATE(vtkImageReslice, ResliceDicom2D);
	IS_Reslicer->SetReslice2D(ResliceDicom2D);
	ResliceDicom2D->Update();

	DataActor->GetMapper()->SetInputConnection(ResliceDicom2D->GetOutputPort());

	VTK_CREATE(vtkRenderer, renderer);
	renderer->AddActor(DataActor);
	renderer->GetActiveCamera()->ParallelProjectionOn();
	renderer->ResetCamera();
	IS_MyCanvas3D->SetRenderer(renderer);

	VTK_CREATE(vtkRenderWindow, window);
	window->AddRenderer(renderer);
	IS_MyCanvas3D->SetWindow(window);

	VTK_CREATE(vtkRenderWindowInteractor, interactor);
	vtkSmartPointer<MyInteractionStyle3D> imageStyle = new MyInteractionStyle3D;
	IS_MyCanvas3D->SetStyle(imageStyle);
	IS_MyCanvas3D->SetInteractor(interactor);
	interactor->SetInteractorStyle(imageStyle);
	window->SetInteractor(interactor);
	imageStyle->SetCurrentRenderer(renderer);
	window->Render();

	//------------------------------------------Algorithm-------------------------------------------
	
	typedef MySpeedFunction3D< InternalImageType, InternalImageType > MySpeedFunction3DType;
	MySpeedFunction3DType::Pointer SegmentationSpeedFunction = MySpeedFunction3DType::New();
	VTK_CREATE(MyCallback3D, IS_Callback);
	IS_Callback->SetImageReslice(ResliceDicom3D, ResliceDicom2D); // "ResliceDicom2D" For overlay
	IS_Callback->SetRenderer(renderer); //For overlay
	IS_Callback->SetWindow(window); //For overlay
	IS_Callback->SetInteractor(interactor);
	IS_Callback->SetDiagram(IS_MyCanvas3D);
	IS_Callback->SetStyle(imageStyle);
	IS_Callback->SetSpeed(SegmentationSpeedFunction);

	imageStyle->AddObserver(vtkCommand::MouseWheelForwardEvent, IS_Callback);
	imageStyle->AddObserver(vtkCommand::MouseWheelBackwardEvent, IS_Callback);
	interactor->AddObserver(vtkCommand::LeftButtonPressEvent, IS_Callback);
	interactor->Start();

	//-------------------------------------------- Combined Manual Refining----------------------------
	//-------------------------------------------------------------------------------------------------

	/*VTK_CREATE(vtkDICOMImageReader, Output_reader);
	Output_reader->SetDirectoryName("E:\\Interactive_Segmentation\\output3D");
	Output_reader->Update();

	Output_reader->GetOutput()->GetExtent(extent);
	Output_reader->GetOutput()->GetSpacing(spacing);
	Output_reader->GetOutput()->GetOrigin(origin);
	Output_reader->GetDataExtent();

	VTK_CREATE(vtkImageReslice, reslice2);
	reslice2->SetInputData(Output_reader->GetOutput());
	reslice2->SetOutputDimensionality(3);
	reslice2->SetResliceAxes(resliceAxes);
	reslice2->SetInterpolationModeToLinear();
	reslice2->Update();


	typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;
	OutputImageType_2_ImageType::Pointer OutputImage_2_Image = OutputImageType_2_ImageType::New();
	OutputImage_2_Image->SetInput(callback->GetAlgorithm()->GetThresholder());
	OutputImage_2_Image->Update();

	typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(OutputImage_2_Image->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	typedef itk::ImageToVTKImageFilter<ImageType>    ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(rescaleFilter->GetOutput());
	connector->Update();



	VTK_CREATE(vtkImageActor, DataActor2);
	MyCanvas3D* diagram2 = new MyCanvas3D();
	diagram2->SetImageData(reslice2->GetOutput());

	VTK_CREATE(vtkImageReslice, Datareslice2);
	Datareslice2->SetInputData(diagram2->GetImage());
	Datareslice2->SetOutputDimensionality(2);
	Datareslice2->SetResliceAxes(resliceAxes);
	Datareslice2->SetInterpolationModeToLinear();
	Datareslice2->Update();
	DataActor2->GetMapper()->SetInputConnection(Datareslice2->GetOutputPort());

	VTK_CREATE(vtkRenderer, renderer2);
	renderer2->AddActor(DataActor2);
	renderer2->GetActiveCamera()->ParallelProjectionOn();
	renderer2->ResetCamera();
	diagram2->SetRenderer(renderer2);

	VTK_CREATE(vtkRenderWindow, window2);
	window2->AddRenderer(renderer2);
	diagram2->SetWindow(window2);

	VTK_CREATE(vtkRenderWindowInteractor, interactor2);
	MyInteractionStyle3D* imageStyle2 = new MyInteractionStyle3D();
	interactor2->SetInteractorStyle(imageStyle2);
	diagram2->SetStyle(imageStyle2);
	diagram2->SetInteractor(interactor2);
	window2->SetInteractor(interactor2);
	imageStyle2->SetCurrentRenderer(renderer2);
	window2->Render();

	VTK_CREATE(MyCallback3D, callback2);
	callback2->SetImageReslice(reslice2, Datareslice2);
	callback2->SetInteractor(interactor2);
	callback2->SetRenderer(renderer2);
	callback2->SetWindow(window2);
	callback2->SetDiagram(diagram2);
	callback2->SetStyle(imageStyle2);

	diagram2->SetInteractionCallback(callback2);

	imageStyle2->AddObserver(vtkCommand::MouseWheelForwardEvent, callback2);
	imageStyle2->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback2);
	interactor2->AddObserver(vtkCommand::InteractionEvent, callback2);
	interactor2->Start();*/




	//------------------------------------------ Write Final ---------------------------------------
	//----------------------------------------------------------------------------------------------

	//std::string outputFilename = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\final_result\\MyVTK1.vtk";
	//
	/*VTKImageToImageType::Pointer vtkImageToImageFilter2 = VTKImageToImageType::New();
	vtkImageToImageFilter2->SetInput(diagram2->GetImage());
	vtkImageToImageFilter2->Update();

	typedef itk::CastImageFilter<ImageType, OutputImageType> OutputImageType_2_ImageType;
	OutputImageType_2_ImageType::Pointer caster_morph_writer = OutputImageType_2_ImageType::New();
	caster_morph_writer->SetInput(vtkImageToImageFilter2->GetOutput());
	caster_morph_writer->Update();

	//typedef  itk::ImageFileWriter< ImageType  > WriterType;
	//WriterType::Pointer writer2 = WriterType::New();
	//writer2->SetFileName(outputFilename);
	//writer2->SetInput(caster_morph_writer->GetOutput());
	//writer2->Update();



	typedef itk::GDCMImageIO    ImageIOType;
	ImageIOType::Pointer morphgdcmIO = ImageIOType::New();
	itk::MetaDataDictionary & dict = morphgdcmIO->GetMetaDataDictionary();
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
	morphgdcmIO->SetSpacing(2, spacing[2]);

	SeriesWriterType::Pointer morphseriesWriter = SeriesWriterType::New();
	morphseriesWriter->SetInput(caster_morph_writer->GetOutput());
	morphseriesWriter->SetImageIO(morph
	IO);

	OutputImageType::RegionType region =
		itk_image->GetOutput()->GetLargestPossibleRegion();
	OutputImageType::IndexType start = region.GetIndex();
	OutputImageType::SizeType  size = region.GetSize();

	const char * outputDirectory = "E:\\Interactive_Segmentation\\output3D\\";
	std::string format = outputDirectory;
	format += "/image%03d.dcm";
	itksys::SystemTools::MakeDirectory(outputDirectory);

	typedef itk::NumericSeriesFileNames             NamesGeneratorType;
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

	namesGenerator->SetSeriesFormat(format.c_str());
	namesGenerator->SetStartIndex(start[2]);
	namesGenerator->SetEndIndex(start[2] + size[2] - 1);
	namesGenerator->SetIncrementIndex(1);

	morphseriesWriter->SetFileNames(namesGenerator->GetFileNames());

	try
	{
		morphseriesWriter->Update();

	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown while writing the series " << std::endl;
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}