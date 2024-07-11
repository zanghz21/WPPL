import sys
sys.path.append('build')
sys.path.append('scripts')
from map import Map
import json

with_wait_costs=True

# map=Map(map_path)
# map.print_graph(map.graph)

# map_json_path = "../maps/competition/online_map/task_dist/test.json"
map_json_path = "../maps/competition/human/pibt_warehouse_33x36_w_mode.json"
with open(map_json_path, "r") as f:
    map_json = json.load(f)
    map_json_str = json.dumps(map_json)

import py_driver # type: ignore # ignore pylance warning
import py_sim

import json
config_path="configs/pibt_default_no_rot.json"

with open("../maps/competition/ours/pibt_warehouse-33x36_w_mode_cma-es_400_agents_four-way-move.json", "r") as f:
    weights_json = json.load(f)
weights = weights_json["weights"]

wait_costs = weights[:948]
edge_weights = weights[948:]
            
with open(config_path) as f:
    config=json.load(f)
    config_str=json.dumps(config)

simulator=py_sim.py_sim(
    # For map, it uses map_path by default. If not provided, it'll use map_json
    # which contains json string of the map
    # map_path=map_path,
    # map_json_str = map_json_str,
    map_json_path = map_json_path,
    simulation_steps=1000,
    update_gg_interval=1000, 
    # for the problem instance we use:
    # if random then we need specify the number of agents and total tasks, also random seed,
    gen_random=True,
    num_agents=400,
    # init_task=True, 
    # init_task_ids=str([719, 1008, 1008, 1008, 1125, 792, 503, 1043, 1151, 468]), 
    # init_agent_pos=str([1123, 485, 165, 694, 718, 357, 890, 845, 640, 623]), 
    # network_params=str([1, ]*2693), 
    num_tasks=100000,
    warmup_steps = 1000, 
    seed=0,
    save_paths=True,
    # weight of the left/right workstation, only applicable for maps with workstations
    left_w_weight=1,
    right_w_weight=1,
    # else we need specify agents and tasks path to load data.
    # agents_path="example_problems/random.domain/agents/random_600.agents",
    # tasks_path="example_problems/random.domain/tasks/random-32-32-20-600.tasks",
    # weights are the edge weights, wait_costs are the vertex wait costs
    # if not specified here, then the program will use the one specified in the config file.
    # weights=compressed_weights_json_str,
    # wait_costs=compressed_wait_costs_json_str, 
    # if we don't load config here, the program will load the default config file.
    config=config_str,    
    # the following are some things we don't need to change in the weight optimization case.
    plan_time_limit=1, # in seconds, time limit for planning at each step, no need to change
    preprocess_time_limit=1800, # in seconds, time limit for preprocessing, no need to change
    file_storage_path="large_files/", # where to store the precomputed large files, no need to change
    task_assignment_strategy="roundrobin", # how to assign tasks to agents, no need to change
    num_tasks_reveal=1, # how many new tasks are revealed, no need to change
)


warmup_r = simulator.warmup()
print(json.loads(warmup_r).keys())
# print("curr pos:", simulator.get_curr_pos())
# raise NotImplementedError
import time
while True:
    start = time.time()
    update_r_str = simulator.update_gg_and_step([1]*3126, [1]*948)
    end = time.time()
    print(end-start)
    update_r = json.loads(update_r_str)
    
    final_pos = update_r["final_pos"]
    final_task = update_r["final_tasks"]
    curr_pos = simulator.get_curr_pos()
    # print("final task:", final_task)
    # print("curr pos:", curr_pos)
    print(update_r["num_task_finished"])
    if update_r["done"]:
        print("done!")
        print(update_r["throughput"])
        
        break       
            