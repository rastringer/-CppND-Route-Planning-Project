#pragma once

#include "render.h"
#include "search_model.h"

class SearchRender : public Render {

    public:
        SearchRender(SearchModel &model);
        void Display( io2d::output_surface &surface );

    private:
        SearchModel &m_Model;

        void DrawStartPosition(io2d::output_surface &surface) const;
        void DrawEndPosition(io2d::output_surface &surface) const;
        void DrawPath(io2d::output_surface &surface) const;
        io2d::interpreted_path PathLine() const;
};