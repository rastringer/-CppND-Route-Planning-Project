#include "search_render.h"



SearchRender::SearchRender(SearchModel & model) : m_Model(model), Render(model) {

}

void SearchRender::Display( io2d::output_surface &surface ) {
    m_Scale = static_cast<float>(std::min(surface.dimensions().x(), surface.dimensions().y()));    
    m_PixelsInMeter = static_cast<float>(m_Scale / m_Model.MetricScale()); 
    m_Matrix = io2d::matrix_2d::create_scale({m_Scale, -m_Scale}) *
               io2d::matrix_2d::create_translate({0.f, static_cast<float>(surface.dimensions().y())});
    Render::Display(surface);
    DrawPath(surface);
    DrawStartPosition(surface);   
    DrawEndPosition(surface);
}

void SearchRender::DrawPath(io2d::output_surface &surface) const{
    io2d::render_props aliased{ io2d::antialias::none };
    io2d::brush foreBrush{ io2d::rgba_color::orange}; 
    float width = 5.0f;
    surface.stroke(foreBrush, PathLine(), std::nullopt, io2d::stroke_props{width});

}

void SearchRender::DrawEndPosition(io2d::output_surface &surface) const{
    io2d::render_props aliased{ io2d::antialias::none };
    io2d::brush foreBrush{ io2d::rgba_color::red };

    auto pb = io2d::path_builder{}; 
    pb.matrix(m_Matrix);

    pb.new_figure({m_Model.end_node.x, m_Model.end_node.y});
    float constexpr l_marker = 0.01f;
    pb.rel_line({l_marker, 0.f});
    pb.rel_line({0.f, l_marker});
    pb.rel_line({-l_marker, 0.f});
    pb.rel_line({0.f, -l_marker});
    pb.close_figure();
    
    surface.fill(foreBrush, pb);
    surface.stroke(foreBrush, io2d::interpreted_path{pb}, std::nullopt, std::nullopt, std::nullopt, aliased);

}

void SearchRender::DrawStartPosition(io2d::output_surface &surface) const{
    io2d::render_props aliased{ io2d::antialias::none };
    io2d::brush foreBrush{ io2d::rgba_color::green };

    auto pb = io2d::path_builder{}; 
    pb.matrix(m_Matrix);

    pb.new_figure({m_Model.start_node.x, m_Model.start_node.y});
    float constexpr l_marker = 0.01f;
    pb.rel_line({l_marker, 0.f});
    pb.rel_line({0.f, l_marker});
    pb.rel_line({-l_marker, 0.f});
    pb.rel_line({0.f, -l_marker});
    pb.close_figure();
    
    surface.fill(foreBrush, pb);
    surface.stroke(foreBrush, io2d::interpreted_path{pb}, std::nullopt, std::nullopt, std::nullopt, aliased);

}

io2d::interpreted_path SearchRender::PathLine() const
{    
    if( m_Model.path.empty() )
        return {};

    const auto nodes = m_Model.path;    
    
    auto pb = io2d::path_builder{};
    pb.matrix(m_Matrix);
    pb.new_figure( ToPoint2D( m_Model.path[0]));

    for( int i=1; i< m_Model.path.size();i++ )
        pb.line( ToPoint2D(m_Model.path[i])); 

      
    return io2d::interpreted_path{pb};
}