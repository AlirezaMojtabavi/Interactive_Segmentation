
#include "stdafx.h"
#include "Canvas2D.h"
#include "Callback2D.h"
#include "Algorithm2D.h"
#include "InteractorStyle2D.h"


int main(int argc, char *argv[])
{

	//-----------------FOR JPEG IMAGES-------------------

	/*VTK_CREATE(vtkJPEGReader, reader);
	reader->SetFileName("C:\\Users\\a.mojtabavi\\Desktop\\New folder\\photo.jpg");
	reader->Update();

	VTK_CREATE(vtkImageLuminance, luminanceFilter);
	luminanceFilter->SetInputConnection(reader->GetOutputPort());
	luminanceFilter->Update();

	std::string outputFilename5 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\my_final.jpg";  // morphology output
	std::string outputFilename7 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\last_curve.jpg"; // manipulate curve*/

	//-----------------FOR DICOM IMAGES--------------------

	auto inputReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	inputReader->SetDirectoryName("E:\\Interactive_Segmentation\\input2D");
	inputReader->Update();

	std::string outputFilename = "E:\\Interactive_Segmentation\\output2D\\final.dcm";  // my output

	auto vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(inputReader->GetOutput());
	vtkImageToImageFilter->Update();

	auto Image_2_InternalImage = ImageType_2_InternalImageType::New();
	Image_2_InternalImage->SetInput(vtkImageToImageFilter->GetOutput());
	Image_2_InternalImage->Update();

	//-------------------------------------------------------
	//---------------------Visualization---------------------
	//-------------------------------------------------------

	auto DataActor = vtkSmartPointer<vtkImageActor>::New();
	Canvas2D* diagram = new Canvas2D();
	diagram->SetImageData(inputReader->GetOutput());
	DataActor->GetMapper()->SetInputData(diagram->GetImage());

	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(DataActor);
	renderer->GetActiveCamera()->ParallelProjectionOn();
	diagram->SetRenderer(renderer);
	renderer->ResetCamera();

	auto window = vtkSmartPointer<vtkRenderWindow>::New();
	window->AddRenderer(renderer);
	diagram->SetWindow(window);

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	InteractorStyle2D* imageStyle = new InteractorStyle2D();
	interactor->SetInteractorStyle(imageStyle);
	diagram->SetStyle(imageStyle);
	diagram->SetInteractor(interactor);
	window->SetInteractor(interactor);
	imageStyle->SetCurrentRenderer(renderer);
	window->Render();

	//----------------------Segmentation------------------

	auto callback = vtkSmartPointer<Callback2D>::New();
	callback->SetInteractor(interactor);
	callback->SetRenderer(renderer);
	callback->SetWindow(window);
	callback->SetDiagram(diagram);
	callback->SetStyle(imageStyle);
	callback->SetOverlayImage(vtkImageToImageFilter->GetOutput());	//overlay
	callback->SetReader(Image_2_InternalImage);

	interactor->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
	interactor->Start();

	//----------------------------------------------------
	//----------------------My Refining-------------------
	//----------------------------------------------------

	OutputImageType_2_ImageType::Pointer mycurve2image = OutputImageType_2_ImageType::New();
	mycurve2image->SetInput(callback->GetAlgorithm()->GetResult());
	mycurve2image->Update();

	ConnectorType::Pointer connector_curve = ConnectorType::New();
	connector_curve->SetInput(mycurve2image->GetOutput());
	connector_curve->Update();

	auto DataActor2 = vtkSmartPointer<vtkImageActor>::New();
	Canvas2D* diagram2 = new Canvas2D();
	diagram2->SetImageData(connector_curve->GetOutput());
	DataActor2->GetMapper()->SetInputData(diagram2->GetImage());

	auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->AddActor(DataActor2);
	renderer2->GetActiveCamera()->ParallelProjectionOn();
	renderer2->ResetCamera();
	diagram2->SetRenderer(renderer2);

	auto window2 = vtkSmartPointer<vtkRenderWindow>::New();
	window2->AddRenderer(renderer2);
	diagram2->SetWindow(window2);

	auto interactor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	InteractorStyle2D* imageStyle2 = new InteractorStyle2D();
	interactor2->SetInteractorStyle(imageStyle2);
	diagram2->SetInteractor(interactor2);
	diagram2->SetStyle(imageStyle2);
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
	vtkImageToImageFilter2->SetInput(diagram2->GetImage());
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

