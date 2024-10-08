#pragma once
#include <osg/ValueObject>
#include <osgEarth/Math>

double haversineDistance(const osg::Vec3d& firstPoint, const osg::Vec3d& lastPoint) {
    const double R = 6371.0;
    double lat1 = firstPoint.x() * M_PI / 180.0;
    double lon1 = firstPoint.y() * M_PI / 180.0;
    double lat2 = lastPoint.x() * M_PI / 180.0;
    double lon2 = lastPoint.y() * M_PI / 180.0;

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1) * cos(lat2) *
        sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double horizontalDistance = R * c * 1000; // Convert to meters

    // Altitude difference
    double altitudeDifference = lastPoint.z() - firstPoint.z();

    // Total distance using Pythagorean theorem
    return sqrt(horizontalDistance * horizontalDistance + altitudeDifference * altitudeDifference);
}