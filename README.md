In the next exercise, you will write a method stub for the A\* search. Before that method can be written, you will need a way to reconstruct the final sequence of nodes found from the `start_node` to the `end_node`, so that the A\* search can store the sequence. The final sequence is reconstructed by starting with the last node that was found, and then iteratively traversing to the parent of that node and each previous node in the sequence until the starting node is reached. In this exercise, you will be writting a method `ConstructFinalPath` that takes a `RouteModel::Node` pointer and iteratively moves the sequence of parents, storing each node in the sequence, until the starting node is reached. 


To complete this exercise, you can use the following steps:
1. Add a `ConstructFinalPath` declaration to the `RoutePlanner` class in `route_planner.h`. This method will be called from the A\* search within the RoutePlanner class, so it can be a private method. `ConstructFinalPath` should accept the pointer `RouteModel::Node *current_node` as the argument, and it should return a vector of ``RouteModel::Node` objects.
2. In `route_planner.cpp` define the `ConstructFinalPath` method. The method should do the following:
  1. Initialize an empty vector `path_found` of `RouteModel::Node` objects and set the class variable `distance` to 0.
  2. Iterate to the parent of each node until a node with parent equal to `nullptr` is reached - this will be the start node. Each node in the iteration should be pushed to the `path_found` vector.
  3. In each step of the iteration, add the distance between a node and its parent to the `distance` variable. 
  4.  
3. In `main.cpp` call `AStarSearch` on the `RoutePlanner` object. This should happen just after the `RoutePlanner` object is defined, but before the `Render render{model}`. 
4. Also in `main.cpp` use the `GetDistance()` method of the `RoutePlanner` object to print the length of the path.
5. Uncomment the following lines in the `Render::Display` method in `render.cpp`. These lines will include the path in the rendered map:

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    RouteModel::Node parent;

    while (current_node->parent != nullptr) {
        path_found.push_back(*current_node);
        parent = *(current_node->parent);
        distance += current_node->distance(parent);
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);
    distance *= m_Model.MetricScale();
    return path_found;
}