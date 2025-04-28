#include <bits/stdc++.h>
#include "fortune.h"

using namespace std;

void computeVoronoiEdgesFromDual(vector<pair<int, int>> &dedges, vector<Point> &points, vector<pair<Point,Point>> &vedges,vector<Point> &vertices) 
{
        int n = points.size();
        long double cenx = 0; long double ceny = 0;
        for(auto &point : points)
        {
            cenx += point.x;
            ceny += point.y;
        }
        cenx = cenx/n; ceny = ceny/n;
        map<int, set<int>> adjacency; 
        for (auto e : dedges) 
        {
            adjacency[e.first].insert(e.second);
            adjacency[e.second].insert(e.first);
        }
        map<tuple<int, int, int>,int> triangles;  
        map<pair<int,int>, vector<pair<Point,int>>> belongs;
        int triangleID = 0;
        for (auto &e : dedges) 
        {
            int a = e.first, b = e.second;
            vector<int> commonNeighbors;
            long double left = 1e12;
            long double right = 1e12;
            for (int x : adjacency[a]) 
            {
                if (adjacency[b].count(x)>0) 
                {
                    int dir = pointSide(points[a],points[b],points[x]);
                    int d = perpendicularDistance(points[a],points[b],points[x]);
                    if(dir>0 && d<right) commonNeighbors.push_back(x);
                    else if(dir<0 && d<left) commonNeighbors.push_back(x);
                }
            }

            for (int c : commonNeighbors) 
            {
                
                vector<int> v = {a,b,c};
                
                sort(v.begin(),v.end());
                
                if(triangles.find({v[0],v[1],v[2]})!=triangles.end())
                    continue;
                
                int tID = triangleID++;
                
                
                triangles[{v[0], v[1], v[2]}] = tID;

                Point s = circumcenter(points[a],points[b],points[c]);

                vertices.push_back(s);

                int t = getTriangleType(points[a],points[b],points[c]);

                belongs[make_pair(v[0], v[1])].push_back({s, (t == 3) ? 1 : 0});
                belongs[make_pair(v[1], v[2])].push_back({s, (t == 1) ? 1 : 0});
                belongs[make_pair(v[0], v[2])].push_back({s, (t == 2) ? 1 : 0});
            }
        }

        for (auto it = belongs.begin(); it != belongs.end(); ++it) 
        {
            // cout<<points[it->first.first].x<<" "<<points[it->first.first].y<<" "<<points[it->first.second].x<<" "<<points[it->first.second].y<<endl;
            // cout<<(it->second).size()<<endl;
            if((it->second).size()==2)
            {
                vedges.push_back({it->second[0].first,it->second[1].first});
            } 
            else
            {
                Point circumcenter = it->second[0].first;
                Point p1 = points[it->first.first];
                Point p2 = points[it->first.second];
                Point midpoint((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
                double dx = p2.x - p1.x;
                double dy = p2.y - p1.y;
                Point direction(-dy, dx);  
                double length = sqrt(direction.x * direction.x + direction.y * direction.y);
                direction.x /= length;
                direction.y /= length;
                // double dotProduct = (midpoint.x - circumcenter.x) * direction.x + 
                //                     (midpoint.y - circumcenter.y) * direction.y;
                double dotProduct = (midpoint.x - cenx) * direction.x + 
                                     (midpoint.y - ceny) * direction.y;
                if (dotProduct < 0) 
                {
                    direction.x = -direction.x;
                    direction.y = -direction.y;
                }
                // if(it->second[0].second == 1)
                // {
                //     direction.x = -direction.x;
                //     direction.y = -direction.y;
                // }
                double extendFactor = 100.0; 
                Point extension = {circumcenter.x + direction.x * extendFactor, 
                                circumcenter.y + direction.y * extendFactor};
                vedges.push_back({circumcenter, extension});
            }
        }


}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    ifstream inFile(argv[1]);
    if (!inFile) 
    {
        cerr << "Error: Could not open input file " << argv[1] << "\n";
        return 1;
    }

    int n;
    inFile >> n;
    
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        inFile >> points[i].x >> points[i].y;
    }

    inFile.close();

    Fortune fortune(points);
    fortune.solve();

    vector<pair<Point,Point>> vedges;
    vector<Point> vertices;
    // saving delauny edges 

    ofstream out("delauny_edges.txt");
    out << "EDGES " << fortune.edges.size() << "\n";
    for (auto &e : fortune.edges) 
    {
        Point p1 = fortune.mp[e.first];
        Point p2 = fortune.mp[e.second];
        out << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << "\n";
    }
    computeVoronoiEdgesFromDual(fortune.edges,points,vedges,vertices);

    ofstream outFile("voronoi_output.txt");

    outFile << "POINTS " << n << "\n";
    for (auto &p : points) 
    {
        outFile << p.x << " " << p.y << "\n";
    }

    outFile << "EDGES " << vedges.size() << "\n";
    for (auto &e : vedges) 
    {
        Point p1 = e.first;
        Point p2 = e.second;
        outFile << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << "\n";
    }

    outFile.close();
    return 0;
}
