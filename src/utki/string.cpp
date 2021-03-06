#include "string.hpp"

#include <cstdarg>
#include <array>

#include "debug.hpp"

using namespace utki;

std::string utki::make_string_va_list(const char* format, va_list args){
	std::array<char, 0x400> buf; // first guess is that the resulting string will take less than 1k

	std::string ret;

	auto buf_ptr = &*buf.begin();
	auto buf_size = buf.size();

	va_list cur_args;
	va_copy(cur_args, args);

	for(unsigned i = 0;; ++i){
		int size = vsnprintf(
				buf_ptr,
				buf_size,
				format,
				cur_args
			);

		if(size < 0){
			throw std::logic_error("snprintf() failed");
		}

		if(decltype(buf_size)(size) >= buf_size){
			// resulting string takes more than we have first guessed, allocate enough memory and try again
			ret.resize(size);

			buf_ptr = &*ret.begin();
			buf_size = ret.size() + 1; // NOTE: C++11 guarantees that std::string's internal buffeer has 1 extra byte for null nerminator.

			va_copy(cur_args, args);
		}else{
			ret = std::string(buf_ptr, size);
			break;
		}

		ASSERT(i < 1)
	}
    
    return ret;
}

std::string utki::make_string(const char* format, ...){
	va_list args;
	va_start(args, format);
	auto ret = make_string_va_list(format, args);
	va_end(args);
	return ret;
}


std::vector<std::string> utki::split(const std::string& str, char delimiter){
	std::vector<std::string> ret;
	size_t pos = 0;

	for(;;){
		auto dpos = str.find(delimiter, pos);

		if(dpos == std::string::npos){
			ret.emplace_back(str.substr(pos, str.length() - pos));
			break;
		}

		ret.emplace_back(str.substr(pos, dpos - pos));

		pos = dpos + 1;
	}

	return ret;
}

std::vector<std::string> utki::word_wrap(const std::string& str, unsigned width){
	std::vector<std::string> ret;

	if(width == 0){
		ret.push_back(str);
		return ret;
	}

	auto line_begin = str.begin();
	auto span_begin = str.begin(); // span is either series of spaces or series of non-spaces
	unsigned word_ended = false; // indicates that at least one word in the current line has ended
	for(auto i = str.begin(); i != str.end(); ++i){
		ASSERT(std::distance(line_begin, i) >= 0)
		if(*i != '\n' && unsigned(std::distance(line_begin, i)) == width){
			if(*span_begin == ' '){ // span of spaces
				if(word_ended){
					ret.push_back(str.substr(
							std::distance(str.begin(), line_begin), 
							std::distance(line_begin, span_begin)
						));
					line_begin = std::next(span_begin);
				}else{
					line_begin = i;
					span_begin = line_begin;
				}
			}else if(word_ended){ // short word span
				ASSERT(std::distance(line_begin, span_begin) >= 1)
				ret.push_back(str.substr(
						std::distance(str.begin(), line_begin), 
						std::distance(line_begin, std::prev(span_begin, 1))
					));
				line_begin = span_begin;
			}else{ // long word span (word longer than width)
				ret.push_back(str.substr(
						std::distance(str.begin(), line_begin),
						std::distance(line_begin, i)
					));
				line_begin = i;
				span_begin = line_begin;
			}
			word_ended = false;
		}
		switch(*i){
			case ' ':
				if(*span_begin != ' '){
					span_begin = i;
					word_ended = true;
				}
				break;
			case '\n':
				//TODO: same as reaching width
				ret.push_back(str.substr(
						std::distance(str.begin(), line_begin),
						std::distance(line_begin, i)
					));
				line_begin = std::next(i);
				span_begin = line_begin;
				word_ended = false;
				break;
			default:
				if(*span_begin == ' '){
					span_begin = i;
				}
				break;
		}
	}

	// add last string
	if(*span_begin == ' '){
		if(word_ended){
			ret.push_back(str.substr(
					std::distance(str.begin(), line_begin),
					std::distance(line_begin, span_begin)
				));
		}
	}else{
		ret.push_back(str.substr(
				std::distance(str.begin(), line_begin),
				std::distance(line_begin, str.end())
			));
	}

	return ret;
}
