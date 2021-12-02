#include <iostream>
#include <vector>
#include <ctime>
#include "sat.h"

using namespace std;

class Gate
{
    public:
        Gate(unsigned i = 0) : _gid(i) {}
        ~Gate() {}

        Var getVar() const { return _var; }
        void setVar(const Var &v) { _var = v; }

    private:
        unsigned _gid; // for debugging purpose...
        Var _var;
};

class Lights
{
    public:
        friend class Gate;

        Lights(int w = 5, int l = 5, bool r = 0) { 
            width = w; length = l; total = w * l; random = r;
            on = new bool*[w];
            for (int i=0; i<w; i++) {
                on[i] = new bool[l];
            }
        }
        ~Lights() {}

        void initCircuit();
        void genProofModel(SatSolver &);
        void reportResult(const SatSolver &, bool);
        Var getoutput() const { return gates[realuse-1] -> getVar(); }

        bool            random;
        bool**          on;

    private:
        int             width;
        int             length;
        int             total;      // total grids
        int             realuse;    // real use of # of gates
        vector<int>     clauses;    // the output gate number of each clause
        vector<Gate *>  gates;
};

void 
Lights::initCircuit()
{
    // Init gates
    for (int i = 0; i < total; i++) {
        gates.push_back(new Gate(i + 1));
        // 1 2 3
        // 4 5 6
        // 7 8 9
    }
    int num_additional_gate = 2*4 + 3*(width + length - 4) + 4*(width - 1)*(length - 1) + (width*length-1);
    for (int i = 0; i < num_additional_gate; i++) {
        gates.push_back(new Gate(total + i));
        // corner : 2
        // side   : 3
        // center : 4
    }
}

void 
Lights::genProofModel(SatSolver &s)
{
    // Allocate and record variables; No Var ID for POs
    for (size_t i = 0, n = gates.size(); i < n; ++i) {
        Var v = s.newVar();
        gates[i]->setVar(v);
    }
    
    int cnt = total; // count additional usage
    // formulate each grids
    for (int i=0; i<total; i++) {
        if (i==0 || i==length-1 || i==total-1 || i==total-length) {
            // corner
            if (i == 0) {
                // left, up
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i+1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i+length]->getVar(), false);
                clauses.push_back(cnt+1);
                cnt += 2;
            } else if (i == length - 1) {
                // right, up
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i+length]->getVar(), false);
                clauses.push_back(cnt+1);
                cnt += 2;
            } else if (i == total - 1) {
                // right, down
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i-length]->getVar(), false);
                clauses.push_back(cnt+1);
                cnt += 2;
            } else {
                // left, down
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i+1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i-length]->getVar(), false);
                clauses.push_back(cnt+1);
                cnt += 2;
            }
        } else if ((i>0 && i<length-1) || (i>total-length && i<total-1) || (i % length == 0) || (i % length == length - 1)) {
            // side
            if (i>0 && i<length-1) {
                // up
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i+1]->getVar(), false);
                s.addXorCNF(gates[cnt+2]->getVar(), gates[cnt+1]->getVar(), false, gates[i+length]->getVar(), false);
                clauses.push_back(cnt+2);
                cnt += 3;
            } else if (i>total-length && i<total-1) {
                // down
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i+1]->getVar(), false);
                s.addXorCNF(gates[cnt+2]->getVar(), gates[cnt+1]->getVar(), false, gates[i-length]->getVar(), false);
                clauses.push_back(cnt+2);
                cnt += 3;
            } else if (i % length == 0) {
                // left
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i+1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i-length]->getVar(), false);
                s.addXorCNF(gates[cnt+2]->getVar(), gates[cnt+1]->getVar(), false, gates[i+length]->getVar(), false);
                clauses.push_back(cnt+2);
                cnt += 3;
            } else {
                // right
                s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-1]->getVar(), false);
                s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i-length]->getVar(), false);
                s.addXorCNF(gates[cnt+2]->getVar(), gates[cnt+1]->getVar(), false, gates[i+length]->getVar(), false);
                clauses.push_back(cnt+2);
                cnt += 3;
            }
        } else {
            // center
            s.addXorCNF(gates[cnt]->getVar(), gates[i]->getVar(), false, gates[i-length]->getVar(), false);
            s.addXorCNF(gates[cnt+1]->getVar(), gates[cnt]->getVar(), false, gates[i-1]->getVar(), false);
            s.addXorCNF(gates[cnt+2]->getVar(), gates[cnt+1]->getVar(), false, gates[i+1]->getVar(), false);
            s.addXorCNF(gates[cnt+3]->getVar(), gates[cnt+2]->getVar(), false, gates[i+length]->getVar(), false);
            clauses.push_back(cnt+3);
            cnt += 4;
        }
    }

    // and each grids up
    s.addAigCNF(gates[cnt]->getVar(), gates[clauses[0]]->getVar(), false, gates[clauses[1]]->getVar(), false);
    cnt++;
    if (!random) {
        for (size_t i=2; i<clauses.size(); i++) {
            s.addAigCNF(gates[cnt]->getVar(), gates[cnt-1]->getVar(), false, gates[clauses[i]]->getVar(), false);
            cnt++;
        }
    } else {
        for (size_t i=2; i<clauses.size(); i++) {
            if (on[i/width][i%width])
                s.addAigCNF(gates[cnt]->getVar(), gates[cnt-1]->getVar(), false, gates[clauses[i]]->getVar(), true);
            else
                s.addAigCNF(gates[cnt]->getVar(), gates[cnt-1]->getVar(), false, gates[clauses[i]]->getVar(), false);
            cnt++;
        }
    }
    realuse = cnt;
}

void 
Lights::reportResult(const SatSolver &solver, bool result)
{
    solver.printStats();
    cout << (result ? "SAT" : "UNSAT") << endl;
    if (result) {
        for (int i=0; i<total; i++) {
            cout << solver.getValue(gates[i]->getVar()) << ' ';
            if (i % length == length - 1)
                cout << endl;
        }
    }
}

int main()
{
    // input width, length
    cout << "Is it random light? Yes-1, No-0" << endl;
    bool random;
    cin >> random;
    cout << "Please given width and length" << endl;
    int width, length;
    cin >> width >> length;
    
    Lights light(width, length, random);
    light.initCircuit();
    
    if (random) {
        for (int i=0; i< width; i++) {
            for (int j=0; j< length; j++) {
                cin >> light.on[i][j];
            }
        }
    }

    // calculate time
    time_t start, end_modeling, start_satsolving, end;
    start = clock();

    SatSolver solver;
    solver.initialize();

    // modeling
    light.genProofModel(solver);
    end_modeling = clock();

    // SAT solving
    start_satsolving = clock();
    bool result;
    solver.assumeRelease(); // Clear assumptions
    solver.assumeProperty(light.getoutput(), true);
    result = solver.assumpSolve();
    light.reportResult(solver, result);
    end = clock();

    cout << "======================" << endl;
    cout << "Time" << endl << "======================" << endl;
    double modeling_time = (end_modeling - start) / CLOCKS_PER_SEC;
    double satsolving_time = (end - start_satsolving) / CLOCKS_PER_SEC;
    cout << "Modeling time : " << modeling_time << endl;
    cout << "SAT solving time : " << satsolving_time << endl;
}