#include <MyRevoke.h>
#include "MyRevoke/Core/EntryPoint.h"

#include "CraftLayer.h"

namespace Revoke {

	class RevokeCraft : public Revoke::Application
	{
	public:
		RevokeCraft()
		{
			PushLayer(new CraftLayer());
		}
		~RevokeCraft()
		{

		}

	private:

	};

	Revoke::Application* Revoke::CreateApplication()
	{
		return new RevokeCraft();
	}
};