#pragma once 
#include <optional> 
#include <QString> 
#include "PlaneGeometry/PlaneOperations.h"

namespace precision_converter { 
using ExactPoint = plane_geometry::Point2D<plane_geometry::ExactScalar>; 
using ExactSegment = plane_geometry::Segment2D<plane_geometry::ExactScalar>;

std::optional<ExactPoint> parsePoint(const QString& text, QString* error); 
std::optional<ExactSegment> parseSegment(const QString& startText, const QString& endText, QString* error);
std::optional<plane_geometry::ExactScalar> parseScalar(const QString& text, QString* error);
QString formatScalar(const plane_geometry::ExactScalar& value, int digits = 50);
}