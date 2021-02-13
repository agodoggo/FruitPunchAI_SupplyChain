#include "TikZGeneration.h"
#include "HardwareUtilities.h" 
#include "Demonstrator.h"
#include "ExactMDPSolver.h"
#include "TabularAgent.h"

void TikZGeneration::outputState(NetworkMDP & mdp, NetworkMDP::State & state, int number)
{
	auto filename = "fig" + std::to_string(number) + ".tex";
	std::ofstream of = HardwareUtilities::GetOutputStreamTo(filename);

	std::cout << filename << std::endl;
	mdp.ToTikzFigure(std::cout, state, 0, true);
	mdp.ToTikzFigure(of, state, 0, true);
}

void TikZGeneration::OutputTrace(std::vector<StateCombination<NetworkMDP>>& trace, NetworkMDP mdp, std::string filename)
{
	/*std::string figureloc = "C:\\Users\\wjaarsveld\\Documents\\Universiteit\\presentaties\\meeting AI\\official\\";
	std::ofstream of;
	std::string outputfile{ figureloc + filename };
	of.open(outputfile);*/

	auto of = HardwareUtilities::GetOutputStreamTo(filename);

	bool first{ true };
	for (auto comb : trace)
	{
		//mdp.ToTikzFigure(std::cout, comb.state, comb.action, comb.event);
		if (mdp.AwaitsAction(comb.state))
		{
			mdp.ToTikzFigure(of, comb.state, comb.action, first);
			first = false;
		}
	}

}
