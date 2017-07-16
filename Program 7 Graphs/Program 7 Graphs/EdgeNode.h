//Tanner Kvarfordt
//A02052217

#ifndef EDGE_NODE_H
#define EDGE_NODE_H

#include <sstream>
#include <assert.h>

class EdgeNode
{
public:

	EdgeNode(char f, char t) : rowSearchedFrom(-1) {
		set(f, t);
	}

	int toNode;  // Subscript of one endpoint in node array.  Nodes are stored as numbers, but printed as characters.
	int fromNode; // Subscript of other endpoint in node array
	int cycleID;  // Cycle which the edge is a member of, -1 if it is included in no cycle
	bool used;    // true if edge is used in final tour
	int row; // row in adjacency matrix
	int col; // col in adjaceny matrix
	int rowSearchedFrom; // the row in the adjacency matrix that the edge was last searched from
	char t; // letter of the node the edge points to
	char f; // letter of the node the edge comes from

				  // Create a string version of Edge
				  // Edge endpoints are stored as numbers, but printed as characters.
	std::string toString()
	{
		std::ostringstream os;  // allows string to act like stream to use stream operations
		char t = toNode + 'A';
		char f = fromNode + 'A';
		os << "   " << f << "-" << t << " (" << cycleID << ")";
		return os.str();
	}

	// if oneNode is an endpoint, return other endpoint
	int getOtherEndpoint(int oneNode)
	{
		if (fromNode == oneNode) return toNode;
		assert(toNode == oneNode);
		return fromNode;
	}

	// Set initial values of an edge from Node f to Node t
	void set(char f, char t)
	{
		this->t = t;
		this->f = f;
		fromNode = f - 'A';
		toNode = t - 'A';
		row = fromNode;
		col = toNode;
		cycleID = -1;
		used = false;
		//cout << "creating Edge " << toString()<<endl;
	}
};

#endif // !EDGE_NODE_H