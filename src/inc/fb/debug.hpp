#ifndef FB_DEBUG_HPP
#define FB_DEBUG_HPP
#include <cstddef>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

// debug utilities written as C++11

namespace fb {
namespace debug {
#ifdef FB_DEBUG

class logger {
public:
	static constexpr struct ws_t {} ws = {};
	static constexpr struct endl_t {} endl = {};

private:
	std::ostream& m_os;
	std::size_t m_count = {};

	explicit logger(std::ostream& os) noexcept :
		m_os{os}
	{}

	void internal_print_ws() {
		for (auto i = std::size_t{}; i < m_count; ++i) {
			m_os << '\t';
		}
	}

	void internal_print_endl() {
		m_os << '\n';
		internal_print_ws();
	}

	template <typename T>
	void internal_print(T&& t) {
		m_os << std::forward<T&&>(t);
	}

	void internal_print(ws_t) {
		internal_print_ws();
	}

	void internal_print(endl_t) {
		internal_print_endl();
	}

	template <typename... Ts>
	void internal_print(ws_t, Ts&&... ts) {
		internal_print_ws();
		internal_print(std::forward<Ts&&>(ts)...);
	}

	template <typename... Ts>
	void internal_print(endl_t, Ts&&... ts) {
		internal_print_endl();
		internal_print(std::forward<Ts&&>(ts)...);
	}

	template <typename T, typename... Ts>
	void internal_print(T&& t, Ts&&... ts) {
		m_os << std::forward<T&&>(t);
		internal_print(std::forward<Ts&&>(ts)...);
	}

public:
	logger() = delete;
	logger(logger const&) = delete;
	logger(logger&&) = delete;

	static logger& instance(std::ostream& os = std::clog) noexcept {
		static thread_local logger inst{os};
		return inst;
	}

	logger& operator=(logger const&) = delete;
	logger& operator=(logger&&) = delete;

	void inc() noexcept {
		++m_count;
	}

	void dec() noexcept {
		if (m_count) {
			--m_count;
		}
	}

	template <typename... Ts>
	void print(Ts&&... ts) {
		internal_print(std::forward<Ts&&>(ts)...);
	}

	class scoped_logger {
	private:
		logger& m_log;

	public:
		using ws_t = logger::ws_t;
		using endl_t = logger::endl_t;

		static constexpr ws_t ws = {};
		static constexpr endl_t endl = {};

		explicit scoped_logger(logger& log, char const* func) :
			m_log{log}
		{
			auto const now = std::chrono::steady_clock::now();

			m_log.print(m_log.ws,
				    "<call \"func\"=",
				    func,
				    "\" time=\"",
				    0,
				    "\" thread_id=\"",
				    std::this_thread::get_id(),
				    "\">\n");

			m_log.inc();
		}

		~scoped_logger() {
			m_log.dec();
			m_log.print(m_log.ws, "</call>\n");
		}

		template <typename... Ts>
		void print(Ts&&... ts) {
			if (sizeof...(ts) > 1) {
				m_log.print(ws, "<log>", endl, std::forward<Ts&&>(ts)..., endl, "</log>\n");
			} else {
				m_log.print(ws, "<log>", std::forward<Ts&&>(ts)..., "</log>\n");
			}
		}
	};

	scoped_logger make_scoped(char const* func = "?") {
		return scoped_logger{*this, func};
	}
};

#endif
} // namespace fb
} // namespace debug

#ifdef FB_DEBUG
#define FB_DEBUG_LOGGER() fb::debug::logger::instance()
#define FB_DEBUG_SCOPED_LOGGER() fb::debug::logger::instance().make_scoped(__func__);
#else
#define FB_DEBUG_LOGGER()
#define FB_DEBUG_SCOPED_LOGGER()
#endif
#endif
