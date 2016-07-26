/*
 * Voice Analysis 
 * Copyright (C) Jérémy DIEUZE, 2016
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "ColorGradient.h"
#include <cassert>

static float ratio_g;
static RGBA_Color first;
static RGBA_Color second;
static RGBA_Color color;
static float width;
static float val;

RGBA_Color ColorGradient::get_color(float value){
    //assert(colors_.size() > 1);

    if (value >= max_){
        return colors_.back();
    }
    ratio_g = value/max_;
    width = 1.0/((float)colors_.size() - 1);
    int i = 0;

    // Find the "bin"
    while (ratio_g > width){
        ratio_g -= width;
        i++;
    }

    ratio_g *= colors_.size() - 1;

    /*assert(0.0 <= ratio_g);
    assert(ratio_g <= 1.0);
    assert(0 <= i);
    assert(i < colors_.size());*/

    first = colors_[i];
    second = colors_[i + 1];

    color.a = 255;
    color.r = interpolate(first.r, second.r, ratio_g);
    color.g = interpolate(first.g, second.g, ratio_g);
    color.b = interpolate(first.b, second.b, ratio_g);

    return color;
}

void ColorGradient::add_color(RGBA_Color color){
    colors_.push_back(color);
}

int ColorGradient::interpolate(int start, int finish, float ratio){
    val = ((float)finish - start) * ratio + ((float) start);

    return (int)(val);
}
