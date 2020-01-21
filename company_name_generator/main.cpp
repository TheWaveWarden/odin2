#include <iostream>
#include <vector>

int main(){
    
    std::vector<std::string> list_of_names = {"henlo", "boiii"};
	
    for(int x = 0; x < list_of_names.size(); ++x){
        for(int y = 0; y < list_of_names.size(); ++y){
            std::cout << list_of_names[x] << " " << list_of_names[y] << std::endl;
        }   
    }
    return 0;
}

