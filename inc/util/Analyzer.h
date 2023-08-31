#pragma once
#include "common.h"
#include "nlohmann/json.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include <fstream>
#include "util/Dev.h"
#include <ctime>
#include <algorithm>

class Analyzer {
public:
    nlohmann::json data;
    string dump_path="analysis/test.csv";

    Analyzer(){
        data["timestamp"]="";
        data["instance"]="";
        data["lifelong_solver"]="";
        data["solver"]="";
        data["single_agent_solver"]="";
        data["planning_window"]=0;
        data["simulation_window"]=0;
        data["timesteps"]=0;
        data["moving_steps"]=0;
        data["finished_tasks"]=0;
        data["details"]="";
    };

    void set_dump_path(const string & path) {
        dump_path=path;
    }

    void init_from_config(const nlohmann::json & config) {
        data["lifelong_solver"]=config.at("lifelong_solver_name");
        data["mapf_solver"]=config.at("solver");
        data["single_agent_solver"]=config.at("single_agent_solver");
        data["planning_window"]=config.at("planning_window");
        data["simulation_window"]=config.at("simulation_window");
        string s=config.dump();
        std::replace(s.begin(),s.end(),',','|');
        // TODO(hj): why not work?
        // std::replace(s.begin(),s.end(),'\\',' ');
        data["details"]=s;
        // TODO
    }

    void timestamp() {
        auto now=time(0);
        auto _time = localtime(&now);
        auto timestamp=boost::format("%d-%d-%d %d:%d:%d")
        %(_time->tm_year+1900)
        %(_time->tm_mon+1)
        %_time->tm_mday
        %_time->tm_hour
        %_time->tm_min
        %_time->tm_sec;
        data["timestamp"]=timestamp.str();
    }

    void dump() {

        auto path=boost::filesystem::path(dump_path);
        auto parent_path=path.parent_path();
        if (!boost::filesystem::exists(parent_path)) {
            boost::filesystem::create_directories(parent_path);
        }

        std::ofstream out;

        vector<std::pair<string,string>> keys={
            {"timestamp","%s"},
            {"instance","%s"},
            {"lifelong_solver","%s"},
            {"mapf_solver","%s"},
            {"single_agent_solver","%s"},
            {"planning_window","%d"},
            {"simulation_window","%d"},
            {"finished_tasks","%d"},
            {"timesteps","%d"},
            {"moving_steps","%d"},
            {"details","%s"}
        };


        string header_template,data_template;
        for (int i=0;i<keys.size();++i) {
            const auto & pair=keys[i];
            if (i!=0) {
                header_template+=",";
                data_template+=",";
            }
            header_template+="%s";
            data_template+=pair.second;
        }

        if (!boost::filesystem::exists(path)) {
            out=std::ofstream(path.c_str(),std::ios::out);    
            //write header
            auto header=boost::format(header_template);
            for (const auto key:keys) {
                header=header%key.first;
            }

            out<<header<<endl;
        } else {
            out=std::ofstream(path.c_str(),std::ios::app);
        }

        auto row=boost::format(data_template);
        for (const auto key:keys){
            row=row%data[key.first];
        }
        out<<row<<endl;
    }
};

extern Analyzer analyzer;