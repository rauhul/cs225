#ifndef MONAD_SHARED
#define MONAD_SHARED

#include "util.h"
#include "pipestream.h"

namespace monad_shared
{
	struct Version
	{
		int32_t major_;
		int32_t minor_;
		int32_t patch_;

		Version(int32_t maj, int32_t min, int32_t pat)
			: major_(maj), minor_(min), patch_(pat)
		{ }

#if 0
		Version(const std::string & s)
		{
			size_t i = 0;
			
		}
#endif

		void print(std::ostream & out) const
		{
			out << major_ << '.' << minor_ << '.' << patch_;
		}
	};

	inline std::ostream & operator<<(std::ostream & out, const Version & v)
	{
		v.print(out);
		return out;
	}

	namespace versioninfo
	{
		extern const char * official_name;
		extern const char * version_name;
		extern const Version version_num;
		extern const char * date;
	}

	void printInfo();

	struct unit_test_input
	{
		typedef void (*function)(util::pipestream &);
		
		private:
		function func_;
		const char * name_;
		long timeout_;
		int32_t points_;
		bool is_asan_;

		public:
		unit_test_input(const char * _name,
		          unit_test_input::function _func,
		          int32_t _points,
		          long _timeout,
		          bool _is_asan)
			: func_(_func),
			  name_(_name),
			  timeout_(_timeout),
			  points_(_points),
			  is_asan_(_is_asan) { }
		
		void execute(util::pipestream & checkstream) const { func_(checkstream); }
		const char * name()        const { return name_;        }
		long         timeout()     const { return timeout_;     }
		int32_t      points()      const { return points_;      }
		bool         is_asan()     const { return is_asan_; }
	};

	struct unit_test_result : public unit_test_input
	{
		private:
		std::string errormsg_;
		std::string output_;
		long time_;

		public:
		unit_test_result(const unit_test_input & _input,
		                 const std::string & _errormsg,
		                 const std::string & _output,
		                 const long _time)
			: unit_test_input(_input),
			  errormsg_((_errormsg != "") ? _errormsg : "Unexpectedly Aborted"),
			  output_(_output),
			  time_(_time) { }

		const std::string & errormsg()      const { return errormsg_;       }
		const std::string & output()        const { return output_;         }
		long               time()           const { return time_;           }

		bool passed() const { return errormsg_ == pass_string; }
		bool failed() const { return !passed(); }
		int32_t points_scored() const { return passed() ? points() : 0; }

		// Static class data
		static const char * pass_string;
		static bool _failed(const unit_test_result & obj) { return obj.failed(); }
	};

	namespace output
	{
		void set_error_message();
		void header(const std::string & title);
		void total_score(int32_t score, int32_t outof);
		void warning(const std::string & message);
		void testname(const unit_test_input & curr_test, int32_t max_testname_len, int32_t max_points_len);
		void detailed_info(const unit_test_result & curr_test);
		std::string passed_string();
		std::string failed_string();
	} // namespace output

}

#endif // MONAD_SHARED
