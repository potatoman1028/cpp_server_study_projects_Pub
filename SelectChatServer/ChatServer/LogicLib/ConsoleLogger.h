#pragma once

#include <mutex>
#include <iostream>

#pragma warning( push )
#pragma warning( disable : 4244 )
//http://cppconlib.codeplex.com/
//http://mariusbancila.ro/blog/2013/08/25/cppconlib-a-cpp-library-for-working-with-the-windows-console/
#include "../../Common/conmanip.h"
using namespace conmanip;
#pragma warning( pop )

#include "../ServerNetLib/ILog.h"

namespace NLogicLib
{
	class ConsoleLog : public NServerNetLib::ILog
	{
	public:
		ConsoleLog() 
		{
			console_out_context ctxout;
			m_Conout = ctxout;
		}

		virtual ~ConsoleLog() {}


	protected:
		virtual void Error(const char* pText) override
		{
			std::lock_guard<std::mutex> guard(m_lock);
			std::cout << settextcolor(console_text_colors::red) << "[ERROR] | " << pText << std::endl;
		}

		virtual void Warn(const char* pText) override
		{
			std::lock_guard<std::mutex> guard(m_lock);
			std::cout << settextcolor(console_text_colors::yellow) << "[WARN] | " << pText << std::endl;
		}

		virtual void Debug(const char* pText) override
		{
			std::lock_guard<std::mutex> guard(m_lock);
			std::cout << settextcolor(console_text_colors::light_white) << "[DEBUG] | " << pText << std::endl;
		}

		virtual void Trace(const char* pText) override
		{
			std::lock_guard<std::mutex> guard(m_lock);
			std::cout << settextcolor(console_text_colors::light_white) << "[TRACE] | " << pText << std::endl;
		}

		virtual void Info(const char* pText) override
		{
			std::lock_guard<std::mutex> guard(m_lock);
			std::cout << settextcolor(console_text_colors::green) << "[INFO] | " << pText << std::endl;
		}

	private:
		console_out m_Conout;
		std::mutex m_lock;

	};
}