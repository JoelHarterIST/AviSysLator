/// @file gncutil.h
/// @brief This file contains the utility functions for GNC.

#ifndef __GNC_UTIL_H__
#define __GNC_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

int16_t phy_to_raw_i16(double phy, double lsb);
double raw_to_phy_i16(int16_t raw, double lsb);
uint8_t lobyte(uint16_t value);
uint8_t hibyte(uint16_t value);
uint16_t makeword(uint8_t lowByte, uint8_t highByte);

#ifdef __cplusplus
} // extern "C"
#endif

#endif