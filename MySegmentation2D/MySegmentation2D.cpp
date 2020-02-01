
#include "stdafx.h"
#include "Canvas2D.h"
#include "Callback2D.h"
#include "Algorithm2D.h"
#include "InteractorStyle2D.h"
#include "MySpeedFunction2D.h"


int main(int argc, char *argv[])
{

	//-----------------FOR JPEG IMAGES-------------------
	//---------------------------------------------------

	/*VTK_CREATE(vtkJPEGReader, reader);
	reader->SetFileName("C:\\Users\\a.mojtabavi\\Desktop\\New folder\\photo.jpg");
	reader->Update();

	VTK_CREATE(vtkImageLuminance, luminanceFilter);
	luminanceFilter->SetInputConnection(reader->GetOutputPort());
	luminanceFilter->Update();

	std::string outputFilename5 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\my_final.jpg";  // morphology output
	std::string outputFilename7 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\last_curve.jpg"; // manipulate curve*/

	//-----------------------------------------------------
	//-----------------FOR DICOM IMAGES--------------------
	//-----------------------------------------------------

	auto input_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	input_reader->SetDirectoryName("E:\\Interactive_Segmentation\\input2D");
	input_reader->Update();

	std::string outputFilename = "E:\\Interactive_Segmentation\\output2D\\final.dcm";  // my Region output

	auto vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(input_reader->GetOutput());
	vtkImageToImageFilter->Update();

	auto Image_2_InternalImage = ImageType_2_InternalImageType::New();
	Image_2_InternalImage->SetInput(vtkImageToImageFilter->GetOutput());
	Image_2_InternalImage->Update();

	//-------------------------------------------------------
	//---------------------Visualization---------------------
	//-------------------------------------------------------

	auto DataActor = vtkSmartPointer<vtkImageActor>::New();
	Canvas2D* diagram = new Canvas2D();
	diagram->SetImageData(input_reader->GetOutput());
	DataActor->GetMapper()->SetInputData(diagram->getImage());

	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(DataActor);
	renderer->GetActiveCamera()->ParallelProjectionOn();
	diagram->set_renderer(renderer);
	renderer->ResetCamera();

	auto window = vtkSmartPointer<vtkRenderWindow>::New();
	window->AddRenderer(renderer);
	diagram->set_window(window);

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	InteractorStyle2D* imageStyle = new InteractorStyle2D();
	interactor->SetInteractorStyle(imageStyle);
	diagram->set_style(imageStyle);
	diagram->set_interactor(interactor);
	window->SetInteractor(interactor);
	imageStyle->SetCurrentRenderer(renderer);
	window->Render();

	//----------------------------------------------------
	//----------------------Segmentation------------------
	//----------------------------------------------------
	typedef MySpeedFunction2D< InternalImageType, InternalImageType > MySpeedFunction2DType;
	MySpeedFunction2DType::Pointer My_Function = MySpeedFunction2DType::New();

	auto callback = vtkSmartPointer<Callback2D>::New();
	callback->SetInteractor(interactor);
	callback->set_renderer(renderer);
	callback->set_window(window);
	callback->SetDiagram(diagram);
	callback->SetStyle(imageStyle);
	callback->set_image(vtkImageToImageFilter->GetOutput());	//overlay
	callback->SetReader(Image_2_InternalImage);
	callback->SetSpeed(My_Function);

	interactor->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
	interactor->Start();

	//----------------------------------------------------
	//----------------------My Refining-------------------
	//----------------------------------------------------

	OutputImageType_2_ImageType::Pointer mycurve2image = OutputImageType_2_ImageType::New();
	mycurve2image->SetInput(callback->GetAlgorithm()->Get_thresholder());
	mycurve2image->Update();

	ConnectorType::Pointer connector_curve = ConnectorType::New();
	connector_curve->SetInput(mycurve2image->GetOutput());
	connector_curve->Update();

	auto DataActor2 = vtkSmartPointer<vtkImageActor>::New();
	Canvas2D* diagram2 = new Canvas2D();
	diagram2->SetImageData(connector_curve->GetOutput());
	DataActor2->GetMapper()->SetInputData(diagram2->getImage());

	auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->AddActor(DataActor2);
	renderer2->GetActiveCamera()->ParallelProjectionOn();
	renderer2->ResetCamera();
	diagram2->set_renderer(renderer2);

	auto window2 = vtkSmartPointer<vtkRenderWindow>::New();
	window2->AddRenderer(renderer2);
	diagram2->set_window(window2);

	auto interactor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	InteractorStyle2D* imageStyle2 = new InteractorStyle2D();
	interactor2->SetInteractorStyle(imageStyle2);
	diagram2->set_interactor(interactor2);
	diagram2->set_style(imageStyle2);
	window2->SetInteractor(interactor2);
	imageStyle2->SetCurrentRenderer(renderer2);
	window2->Render();

	auto callback2 = vtkSmartPointer<Callback2D>::New();
	callback2->SetInteractor(interactor2);
	callback2->SetDiagram(diagram2);
	callback2->SetStyle(imageStyle2);

	interactor2->AddObserver(vtkCommand::InteractionEvent, callback2);
	interactor2->Start();

	//--------------------------------------------------------
	//----------------------My Morphology---------------------
	//--------------------------------------------------------

	VTKImageToImageType::Pointer vtkImageToImageFilter2 = VTKImageToImageType::New();
	vtkImageToImageFilter2->SetInput(diagram2->getImage());
	vtkImageToImageFilter2->Update();

	ImageType_2_InternalImageType::Pointer Image_2_InternalImage2 = ImageType_2_InternalImageType::New();
	Image_2_InternalImage2->SetInput(vtkImageToImageFilter2->GetOutput());
	Image_2_InternalImage2->Update();

	typedef itk::BinaryFillholeImageFilter< InternalImageType > FilterType;
	FilterType::Pointer morph_filter = FilterType::New();
	morph_filter->SetInput(Image_2_InternalImage2->GetOutput());
	morph_filter->SetForegroundValue(255);

	InternalImageType_2_OutputImageType::Pointer morph_cast = InternalImageType_2_OutputImageType::New();
	morph_cast->SetInput(morph_filter->GetOutput());
	morph_cast->Update();

	WriterType::Pointer mywriter = WriterType::New();
	mywriter->SetFileName(outputFilename.c_str());
	mywriter->SetInput(morph_cast->GetOutput());
	try
	{
		mywriter->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		std::cerr << "Error: " << error << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

