# EDA_HW2
電子自動化設計作業二 包含作業一
將ISCAS-89 s208.1的netlist儲存並執行K-L algorithm  
終止條件為當有分割到出現node數小於自訂數K為止  
https://web.eecs.umich.edu/~jhayes/iscas.restore/s208_1.html  
測試環境: https://www.onlinegdb.com/online_c++_compiler C++14以上(含C++14)  
K-L algorithm:  
Kernighan and Lin, “An efficient heuristic procedure forpartitioning graphs,” The Bell System Technical Journal, vol. 49,no. 2, Feb. 1970.  
An iterative, 2-way, balanced partitioning (bi-sectioning)heuristic.  
Till the cut size keeps decreasing  
Vertex pairs which give the largest decrease or the smallestincrease in cut size are exchanged.  
These vertices are then locked (and thus are prohibited fromparticipating in any further exchanges).  
This process continues until all the vertices are locked.  
Find the set with the largest partial sum for swapping.  
Unlock all vertices.
