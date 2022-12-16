#pragma once

#include "math/Math.h"
#include "math/Color.h"

#include <vector>

namespace engine::hammer
{
    using Color = ColorRGBA<byte>;

    struct Editor
    {
        Color color;
        int visgroupid;
        int groupid;
        bool visgroupshown;
        bool visgroupautoshown;
        const char* comments = nullptr;
        // logicalpos
    };

    struct MapEntity
    {
        int id;
        const char* classname = nullptr;
        int spawnflags = 0;
        // connections {}
        // solid {}
        Editor editor;
        // hidden {}
    };

    struct Plane
    {
        vec3 points[3];
    };

    struct Side
    {
        int id;
        const char* material;
        // uaxis
        // vaxis
        float rotation;
        float lightmapscale;
        float smoothing_groups;
        // dispinfo {}
    };

    struct Solid
    {
        int id;
        std::vector<Side> sides;
        Editor editor;
    };
    
    struct World : MapEntity
    {
        int mapversion = 0;
        const char* skyname = nullptr;

        std::vector<Solid> solids;
    };

    struct Visgroup
    {
        const char* name;
        int id;
        Color color;
        std::vector<Visgroup> children;
    };

    struct VMF
    {
        // versioninfo {}
        int editorversion = 400;
        int editorbuild = 9001;
        int mapversion = 0;
        int formatversion = 100;
        bool prefab = false;

        // viewsettings {}
        bool bSnapToGrid = true;
        bool bShowGrid = true;
        bool bShowLogicalGrid = false;
        int nGridSpacing = 64;
        bool bShow3DGrid = false;

        World world;
        std::vector<Visgroup> visgroups;
        std::vector<MapEntity> entities;
        // hidden {}
        // cameras {}
        // cordon {}
    };
}