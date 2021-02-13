#include "SLAInvAgent.h"
#include <cmath>

SLAInvAgent::SLAInvAgent(const SLAInvMDP* mdp_ptr):mdp_ptr{ mdp_ptr }
{
}

size_t SLAInvAgent::GetAction(const SLAInvState& state) const
{
    


    double EBO = 0.0;
    int S = -1;
    double CritFrac = mdp_ptr->p / (mdp_ptr->p + mdp_ptr->h);     //Probability that demand is satisfied > CritFrac
    double lambda = mdp_ptr->ExpectedDemand * mdp_ptr->Leadtime;
    //unsigned long long factorial = 1;
    //double probability = 1.00;  //Pr{X = x+S}

    double EBOaim = 0.0;
    double EBO_S = 10000.0;


    std::vector<double> DemandProbs;
    DemandProbs = mdp_ptr->DemandProbs;

    std::vector<double> DemOverLeadtimeProbs;
    DemOverLeadtimeProbs = mdp_ptr->DemOverLeadtimeProbs;
    
    int i = 0;
    size_t MaxOrderSize{ 0 };

    double CDF_DemOverLeadtime = 0.0;
    double CDF = 0.0;
    int SafetyStock = ceil(sqrt(mdp_ptr->ExpectedDemand * mdp_ptr->Leadtime));

    if (mdp_ptr->HorizonLenght * mdp_ptr->EBOTarget > state.CumBackorders) { //EBO target not exceeded 
        EBOaim = (mdp_ptr->HorizonLenght * mdp_ptr->EBOTarget - state.CumBackorders) / state.DaysRemaining;
        //std::cout << "CumBO = " << state.CumBackorders << "\t DaysRemaining: " << state.DaysRemaining << "\t CritFrac = " << CritFrac;
        //std::cout << "\nEBOtarget = " << mdp_ptr->EBOTarget << "\t EBOaim = " << EBOaim << "\t EBOaim*Leadtime = " << EBOaim * mdp_ptr->Leadtime << std::endl;
        /*
        while (EBO_S >= EBOaim) { //find minimal S that is required for certain EBOaim, S +=1 until EBO_S is smaller than EBOaim
            S += 1;
            EBO = 0.0;
            for (i = S; i < DemOverLeadtimeProbs.size(); i++) {
                EBO += ((i - S) * DemOverLeadtimeProbs[i]);
            }
            EBO_S = EBO;
        }
        */
   
                                        //static_cast<double>(mdp_ptr->Leadtime)
        double CritFracPeriod = pow(CritFrac, (1 / static_cast<double>(state.DaysRemaining)));
        //std::cout << state.DaysRemaining << "\tFraction: " << CritFrac << "\tCritFracPeriod: " << CritFracPeriod ;
        while (CDF < CritFracPeriod){ //CritFracPeriod
            S += 1;
            CDF_DemOverLeadtime = 0;
            //std::cout << "S = " << S << "\t S+EBOaim*Leadtime = " << S + (EBOaim * mdp_ptr->Leadtime);
            for (i = 0; i <= S + (EBOaim * mdp_ptr->Leadtime) && i < (DemOverLeadtimeProbs.size() - 1); i++) {       //Prob. that Demand over leadtime > S + EBO allowance should be smaller than p/p+h 
                CDF_DemOverLeadtime += DemOverLeadtimeProbs[i];
            }
            //std::cout << "\t CDF_DemOverLeadtime = " << CDF_DemOverLeadtime << std::endl;
            CDF = CDF_DemOverLeadtime;
        }
        
 
    }
    else {      //EBO target exceeded already
        S = mdp_ptr->MaxInvInSystem; //MaxOrderSize(Leadtime*ExpDemand)//Required basestock level S should be equal to k in order to have probability of satisfying demand is >= p/p+h
        //std::cout << "EBO exceeded: S = " << S << std::endl;
    }

    
	size_t action = S;
    //std::cout << "Reorder level S = " << action << std::endl;
	return action;
}