#include "Util.hpp"

#include <Windows.h>



namespace comm
{
std::string errorAsString(DWORD error)
{
	if (!error)
		return std::string();

	LPSTR messageBuffer{ nullptr };
	TRY(FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, error, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr));

	std::string message{ messageBuffer };

	LocalFree(messageBuffer);

	return message;
}

double distance(POINT a, POINT b)
{
	LONG dx{ a.x - b.x };
	LONG dy{ a.y - b.y };
	return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}
}
