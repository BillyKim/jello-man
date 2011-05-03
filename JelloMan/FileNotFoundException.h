#include "D3DUtil.h"

namespace exceptions
{
	class FileNotFoundException
	{
	public:
		FileNotFoundException();
		FileNotFoundException(const tstring& filename);

		const tstring& GetMessage() { return m_Message; }
	private:
		tstring m_Message;
	};
}