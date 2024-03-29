#include "vtkSmartPointer.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkActor.h"

#include "vtkDICOMImageReader.h"
#include "vtkImageViewer2.h"
#include "vtkInteractorStyleImage.h"

class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	static myVtkInteractorStyleImage* New();
	vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
	vtkImageViewer2* ImageViewer;
	int Slice;
	int MinSlice;
	int MaxSlice;

public:
	void SetImageViewer(vtkImageViewer2* imageViewer)
	{
		this->ImageViewer = imageViewer;
		this->MinSlice = imageViewer->GetSliceMin();
		this->MaxSlice = imageViewer->GetSliceMax();
		this->Slice = (this->MinSlice + this->MaxSlice) / 2;
		this->ImageViewer->SetSlice(this->Slice);
		this->ImageViewer->Render();
	}

protected:
	virtual void OnMouseWheelForward()
	{
		if (this->Slice < this->MaxSlice)
		{
			this->Slice += 1;
			this->ImageViewer->SetSlice(this->Slice);
			this->ImageViewer->Render();
		}
	}

	virtual void OnMouseWheelBackward()
	{
		if (this->Slice > this->MinSlice)
		{
			this->Slice -= 1;
			this->ImageViewer->SetSlice(this->Slice);
			this->ImageViewer->Render();
		}
	}
};

vtkStandardNewMacro(myVtkInteractorStyleImage);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " DicomDirectory" << std::endl;
		return EXIT_FAILURE;
	}

	auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(argv[1]);
	reader->Update();

	auto imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(interactor);
	imageViewer->SetSize(400, 400);
	imageViewer->SetColorLevel(500);
	imageViewer->SetColorWindow(1000);
	imageViewer->SetSliceOrientationToXY();
	imageViewer->GetRenderer()->SetBackground(1, 1, 1);

	auto myStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myStyle->SetImageViewer(imageViewer);
	interactor->SetInteractorStyle(myStyle);

	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	interactor->Start();

	return EXIT_SUCCESS;
}
