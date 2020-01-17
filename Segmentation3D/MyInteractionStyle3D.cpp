
#include "stdafx.h"
#include "MyInteractionStyle3D.h"



void MyInteractionStyle3D::OnKeyPress()
{

	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	if (key == "c") //contrast
		flag = -1;
	if (key == "b") // brush - select seed point
		flag = 1;
	if (key == "r") //manual refine
		flag = 2;
	if (key == "p") //manual erase
		flag = 3;
	if (key == "g") // go - run
		flag = 4;
	if (key == "o") // remove seed
		flag = 5;

}

int MyInteractionStyle3D::GetFlag()
{
	return flag;
}

void MyInteractionStyle3D::WindowLevel()
{
	if (flag == -1)
	{
		vtkRenderWindowInteractor *rwi = this->Interactor;

		this->WindowLevelCurrentPosition[0] = rwi->GetEventPosition()[0];
		this->WindowLevelCurrentPosition[1] = rwi->GetEventPosition()[1];

		if (this->HandleObservers &&
			this->HasObserver(vtkCommand::WindowLevelEvent))
		{
			this->InvokeEvent(vtkCommand::WindowLevelEvent, this);
		}
		else if (this->CurrentImageProperty)
		{
			int *size = this->CurrentRenderer->GetSize();

			double window = this->WindowLevelInitial[0];
			double level = this->WindowLevelInitial[1];

			// Compute normalized delta

			double dx = (this->WindowLevelCurrentPosition[0] -
				this->WindowLevelStartPosition[0]) * 4.0 / size[0];
			double dy = (this->WindowLevelStartPosition[1] -
				this->WindowLevelCurrentPosition[1]) * 4.0 / size[1];

			// Scale by current values

			if (fabs(window) > 0.01)
			{
				dx = dx * window;
			}
			else
			{
				dx = dx * (window < 0 ? -0.01 : 0.01);
			}
			if (fabs(level) > 0.01)
			{
				dy = dy * level;
			}
			else
			{
				dy = dy * (level < 0 ? -0.01 : 0.01);
			}

			// Abs so that direction does not flip

			if (window < 0.0)
			{
				dx = -1 * dx;
			}
			if (level < 0.0)
			{
				dy = -1 * dy;
			}

			// Compute new window level

			double newWindow = dx + window;
			double newLevel = level - dy;

			if (newWindow < 0.01)
			{
				newWindow = 0.01;
			}

			this->CurrentImageProperty->SetColorWindow(newWindow);
			this->CurrentImageProperty->SetColorLevel(newLevel);

			this->Interactor->Render();

		}
	}
}
