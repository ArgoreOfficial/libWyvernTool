#pragma once

#include <memory>
#include <string>
#include <stdexcept>

namespace wv {

template<typename... Args>
inline std::string format( const std::string& _format, Args... _args )
{
	if constexpr( sizeof...( Args ) == 0 )
		return _format; // no need to format

	int size_s = std::snprintf( nullptr, 0, _format.c_str(), _args... ) + 1;
	if( size_s <= 0 )
		throw std::runtime_error( "Error during formatting." );
	size_t size = static_cast<size_t>( size_s );

	std::unique_ptr<char[]> buf( new char[ size ] );
	std::snprintf( buf.get(), size, _format.c_str(), _args... );
	return std::string( buf.get(), buf.get() + size - 1 );
}

}

