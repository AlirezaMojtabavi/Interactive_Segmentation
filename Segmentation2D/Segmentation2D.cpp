// Interactive_Segmentation.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "Canvas2D.h"
#include "Callback2D.h"
#include "Algorithm2D.h"
#include "InteractorStyle2D.h"
#include "SpeedFunction2D.h"
#include "ImageTypeDetails2D.h"


int main(int argc, char* argv[])
{

	//-----------------FOR JPEG IMAGES-------------------
	//---------------------------------------------------

	/*VTK_CREATE(vtkJPEGReader, reader);
	reader->SetFileName("C:\\Users\\a.mojtabavi\\Desktop\\New folder\\photo.jpg");
	reader->Update();

	VTK_CREATE(vtkImageLuminance, luminanceFilter);
	luminanceFilter->SetInputConnection(reader->GetOutputPort());
	luminanceFilter->Update();

	std::string outputFilename1 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\my_curve.jpg"; // level set output
	std::string outputFilename3 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\threshold_curve.jpg"; // prievous curve
	std::string outputFilename5 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\my_final.jpg";  // morphology output
	std::string outputFilename6 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\threshold_final.jpg"; // prievous region
	std::string outputFilename7 = "C:\\Users\\a.mojtabavi\\Desktop\\final_write\\last_curve.jpg"; // manipulate curve*/

	//-----------------------------------------------------
	//-----------------FOR DICOM IMAGES--------------------
	//-----------------------------------------------------

	VTK_CREATE(vtkDICOMImageReader, reader);
	reader->SetDirectoryName("E:\\Interactive_Segmentation\\input2D");
	reader->Update();

	std::string outputFilename = "E:\\Interactive_Segmentation\\output2D\\my_final.dcm";  // my Region output

	typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;
	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(reader->GetOutput());
	vtkImageToImageFilter->Update();

	typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalImageType;
	ImageType_2_InternalImageType::Pointer itk_image = ImageType_2_InternalImageType::New();
	itk_image->SetInput(vtkImageToImageFilter->GetOutput());
	itk_image->Update();

	//-------------------------------------------------------
	//---------------------Visualization---------------------
	//-------------------------------------------------------

	VTK_CREATE(vtkImageActor, DataActor);
	Canvas2D* diagram = new Canvas2D();
	diagram->SetImageData(reader->GetOutput());
	DataActor->GetMapper()->SetInputData(diagram->getImage());

	VTK_CREATE(vtkRenderer, renderer);
	renderer->AddActor(DataActor);
	renderer->GetActiveCamera()->ParallelProjectionOn();
	diagram->set_renderer(renderer);
	renderer->ResetCamera();

	VTK_CREATE(vtkRenderWindow, window);
	window->AddRenderer(renderer);
	diagram->set_window(window);

	VTK_CREATE(vtkRenderWindowInteractor, interactor);
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
	typedef SpeedFunction2D< InternalImageType, InternalImageType > MySpeedType;
	MySpeedType::Pointer My_Function = MySpeedType::New();

	VTK_CREATE(Callback2D, callback);
	callback->SetInteractor(interactor);
	callback->set_renderer(renderer);
	callback->set_window(window);
	callback->SetDiagram(diagram);
	callback->SetStyle(imageStyle);
	callback->SetReader(itk_image->GetOutput());
	callback->SetSpeed(My_Function);

	interactor->AddObserver(vtkCommand::InteractionEvent, callback);
	interactor->Start();

	//----------------------------------------------------
	//----------------------My Refining-------------------
	//----------------------------------------------------

	typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;
	OutputImageType_2_ImageType::Pointer mycurve2image = OutputImageType_2_ImageType::New();
	mycurve2image->SetInput(callback->GetAlgorithm()->Get_thresholder());
	mycurve2image->Update();

	typedef itk::ImageToVTKImageFilter<ImageType>  ConnectorType;
	ConnectorType::Pointer connector_curve = ConnectorType::New();
	connector_curve->SetInput(mycurve2image->GetOutput());
	connector_curve->Update();

	VTK_CREATE(vtkImageActor, DataActor2);
	Canvas2D* diagram2 = new Canvas2D();
	diagram2->SetImageData(connector_curve->GetOutput());
	DataActor2->GetMapper()->SetInputData(diagram2->getImage());

	VTK_CREATE(vtkRenderer, renderer2);
	renderer2->AddActor(DataActor2);
	renderer2->GetActiveCamera()->ParallelProjectionOn();
	renderer2->ResetCamera();
	diagram2->set_renderer(renderer2);

	VTK_CREATE(vtkRenderWindow, window2);
	window2->AddRenderer(renderer2);
	diagram2->set_window(window2);

	VTK_CREATE(vtkRenderWindowInteractor, interactor2);
	InteractorStyle2D* imageStyle2 = new InteractorStyle2D();
	interactor2->SetInteractorStyle(imageStyle2);
	diagram2->set_interactor(interactor2);
	diagram2->set_style(imageStyle2);
	window2->SetInteractor(interactor2);
	imageStyle2->SetCurrentRenderer(renderer2);
	window2->Render();

	VTK_CREATE(Callback2D, callback2);
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

	ImageType_2_InternalImageType::Pointer caster_image2Internal = ImageType_2_InternalImageType::New();
	caster_image2Internal->SetInput(vtkImageToImageFilter2->GetOutput());
	caster_image2Internal->Update();

	typedef itk::BinaryFillholeImageFilter< InternalImageType > FilterType;
	FilterType::Pointer morph_filter = FilterType::New();
	morph_filter->SetInput(caster_image2Internal->GetOutput());
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




