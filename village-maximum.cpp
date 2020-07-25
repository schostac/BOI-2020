#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <cmath>
#include <cstdint>

using Graph = std::vector<std::vector<int>>;
using Edges = std::vector<std::pair<int, int>>;

void setSubtreeSizes(const Graph& graph, std::vector<bool>& vis, std::vector<int>& sizes, const int u)
{
    vis[u] = true;
    sizes[u] = 1;
    
    for(const auto v : graph[u]) {
        if(not vis[v]) {
            setSubtreeSizes(graph, vis, sizes, v);
            sizes[u] += sizes[v];
        }
    }
}

void setNodesPreorder(const Graph& graph, std::vector<bool>& vis, std::vector<int>& ord, const int u)
{
    vis[u] = true;
    ord.emplace_back(u);

    for(const auto v: graph[u]) {
        if (not vis[v]) {
            setNodesPreorder(graph, vis, ord, v);
        }
    }
}

std::vector<int> calcSubtreeSizes(const Graph& graph)
{
    std::vector<bool> vis(graph.size());
    std::vector<int> sizes(graph.size()); 
    setSubtreeSizes(graph, vis, sizes, 0);
    return sizes;
}

std::int64_t calcLargestTotalLength(const Graph& graph, const Edges& edges, const std::vector<int>& sizes)
{
    std::int64_t total = 0;
    const int n = graph.size();
    assert(edges.size() == n-1);

    for(const auto& [u, v] : edges) {
        const int subtreeSize1 = std::min(sizes[u], sizes[v]);
        const int subtreeSize2 = n - subtreeSize1;

        assert(std::min(subtreeSize1, subtreeSize2) != 0);
        total += std::min(subtreeSize1, subtreeSize2);
    }

    return total * 2; // Because edges store only those in one direction
}

std::vector<int> calcNodesPreorder(const Graph& graph)
{
    std::vector<int> order;
    std::vector<bool> vis(graph.size());

    setNodesPreorder(graph, vis, order, 0);
    assert(order.size() == graph.size());

    return order;
}

std::vector<int> calcNewPlacesForNodes(const std::vector<int>& order)
{
    const int n = order.size();
    std::vector<int> result(n, -1);

    for(int i = 0; i < n; ++i) {
        const auto from = i;
        const auto to = (i+n/2) % n;
        result[order[from]] = order[to];
    }

    return result;
}

int main()
{
    std::ios_base::sync_with_stdio(false); 
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n;
    std::cin >> n;

    Graph graph(n);
    Edges edges;

    for(int i = 0; i < n-1; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u, --v;
        graph[u].emplace_back(v);
        graph[v].emplace_back(u);
        edges.emplace_back(u, v);
    }

    const auto sizes = calcSubtreeSizes(graph);
    const auto total = calcLargestTotalLength(graph, edges, sizes);
    const auto order = calcNodesPreorder(graph);
    const auto result = calcNewPlacesForNodes(order);

    assert(total > 0 and result.size() == graph.size());

    std::cout << total << std::endl;
    for(const auto u : result) {
        assert(u != -1);
        std::cout << u + 1 << ' ';
    }
    std::cout << std::endl;

    return 0;
}
