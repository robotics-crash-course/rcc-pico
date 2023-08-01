// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * differentiator.cpp -- differentiator
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "differentiator.h"


Differentiator::Differentiator(float sigma, float ts)
{
	this->sigma = sigma;
	this->ts    = ts;

	beta = (2.0 * sigma - ts) / (2.0 * sigma + ts);
}

/// @brief 
/// @param y the incoming sample to use for differentiation
/// @return the band limited derivative at this sample
float Differentiator::differentiate(float y)
{
	// calculate derivative
	y_dot = (beta * y_dot) + (((1 - beta) / ts) * (y - y_d1));

	y_d1 = y;

	return y_dot;
}

/// @brief 
/// @param degrees the value we want the "previous sample" to have after the reset
void Differentiator::reset(float degrees)
{
	y_dot = 0;
	y_d1  = degrees;
}

/// @brief 
/// @param ts : sample time (s)
/// @param sigma  : bandwidth of the low pass filter applied (1/cutoff-freq)
void Differentiator::setTimeParameters(float ts, float sigma)
{
	this->ts    = ts;
	this->sigma = sigma;

	beta = (2.0 * sigma - ts) / (2.0 * sigma + ts);
}
