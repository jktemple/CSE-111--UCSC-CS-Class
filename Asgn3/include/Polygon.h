/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <vector>

#include "Containable.h"
#include "Point.h"

class Polygon : public Containable {
  public:
    // do not modify or remove this constructor
    std::vector<Point> vertices;
    Polygon(std::vector<Point> vertices);

    // do not modify or remove these functions
    bool ContainedBy(Circle &circle);
    bool ContainedBy(Polygon &polygon);
    bool ContainedBy(ReuleauxTriangle &rt);
};

#endif
