/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 * bits.h -- bit vector manipulation
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

/**
 * @file bits.h
 * @brief bit vector manipulation
 */

#ifndef _BITS_H_
#define _BITS_H_

#include <sys/types.h>
#include <stdint.h>

/**
 * Sets the bit @p bit in bit-vector @p vec. This function returns @c
 * 1 if bit was set or @c -1 on error (i.e. when the given bit does
 * not fit in the vector).
 *
 * @param vec  The bit-vector to change.
 * @param size The size of @p vec in bytes.
 * @param bit  The bit to set in @p vec.
 *
 * @return @c -1 if @p bit does not fit into @p vec, @c 1 otherwise.
 */
inline static int bits_setb(uint8_t *vec, size_t size, uint8_t bit)
{
	if (size <= (size_t)(bit >> 3)) {
		return -1;
	}

	*(vec + (bit >> 3)) |= (uint8_t)(1 << (bit & 0x07));
	return 1;
}

/**
 * Clears the bit @p bit from bit-vector @p vec. This function returns
 * @c 1 if bit was cleared or @c -1 on error (i.e. when the given bit
 * does not fit in the vector).
 *
 * @param vec  The bit-vector to change.
 * @param size The size of @p vec in bytes.
 * @param bit  The bit to clear from @p vec.
 *
 * @return @c -1 if @p bit does not fit into @p vec, @c 1 otherwise.
 */
inline static int bits_clrb(uint8_t *vec, size_t size, uint8_t bit)
{
	if (size <= (size_t)(bit >> 3)) {
		return -1;
	}

	*(vec + (bit >> 3)) &= (uint8_t)(~(1 << (bit & 0x07)));
	return 1;
}

/**
 * Gets the status of bit @p bit from bit-vector @p vec. This function returns
 * @c 1 if the bit is set, @c 0 otherwise (even in case of an error)..
 *
 * @param vec  The bit-vector to read from..
 * @param size The size of @p vec in bytes.
 * @param bit  The bit to get from @p vec.
 *
 * @return @c 1 if the bit is set, @c 0 otherwise.
 */
inline static int bits_getb(const uint8_t *vec, size_t size, uint8_t bit)
{
	if (size <= (size_t)(bit >> 3)) {
		return -1;
	}

	return (*(vec + (bit >> 3)) & (1 << (bit & 0x07))) != 0;
}

#endif							/* _BITS_H_ */
