#pragma once

namespace crc32
{
	void init( unsigned int* crc );
	void process_buffer( unsigned int* crc, const void* p, int len );
	void end( unsigned int* crc );
	unsigned int get_table_entry( unsigned int slot );

	inline unsigned int process_single_buffer( const void* p, int len )
	{
		unsigned int crc;
		crc32::init( &crc );
		crc32::process_buffer( &crc, p, len );
		crc32::end( &crc );
		return crc;
	}
} // namespace crc32
