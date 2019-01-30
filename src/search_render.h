#pragma once

#include "render.h"
#include "search_model.h"

class SearchRender : public Render {

    public:
        SearchRender(SearchModel &model);
        void Display( io2d::output_surface &surface );

    private:
        SearchModel &m_Model;
        float m_Scale = 1.f;
        float m_PixelsInMeter = 1.f;
        io2d::matrix_2d m_Matrix;

        void DrawStartPosition(io2d::output_surface &surface) const;
        void DrawEndPosition(io2d::output_surface &surface) const;
        void DrawPath(io2d::output_surface &surface) const;
        io2d::interpreted_path PathLine() const;
};