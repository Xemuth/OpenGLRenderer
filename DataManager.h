#ifndef _pluginUFE_OpenGLRenderer_DataManager_h_
#define _pluginUFE_OpenGLRenderer_DataManager_h_


namespace Upp{
	
template <class T>
class DataManager{
	public:
		DataManager(){}
		~DataManager(){
			map.Clear();
		}
	
		T& Create(const String& name){
			int position = map.Find(name);
			if(position == -1){
				return map.Add(name);
			}
			throw Exc("DataManager exception : Can't Create object of type " + String(typeid(T).name())  + " named '"+ name +"' because it already exist");
		}
				
		T& Get(const String& name){
			int position = map.Find(name);
			if(position != -1){
				return map[position];
			}
			throw Exc("DataManager exception : Can't retrieve object of type " + String(typeid(T).name())  + " named '"+ name +"' because it don't exist");
		}
		
		T& Get(int position){
			if(position < 0 || position >= map.GetCount())throw Exc("DataManager exception : position " + AsString(position) + " is out of bound");
				return map[position];
		}
		
		const T& Get(const String& name)const{
			int position = map.Find(name);
			if(position != -1){
				return map[position];
			}
			throw Exc("DataManager exception : Can't retrieve object of type " + String(typeid(T).name())  + " named '"+ name +"' because it don't exist");
		}
		
		const T& Get(int position)const{
			if(position < 0 || position >= map.GetCount())throw Exc("DataManager exception : position " + AsString(position) + " is out of bound");
				return map[position];
		}
		
		int GetCount()const{
			return map.GetCount();
		}
		
		int GetPosition(const String& name){
			return map.Find(name);
		}
		
		bool Has(const String& name){
			return (map.Find(name) != -1);
		}
		
		bool Remove(const String& name){
			int position = map.Find(name);
			if(position != -1){
				//Then we remove it
				map.Remove(position);
				return true;
			}
			return false;
		}
		
	private:
		friend class SGLRenderer;
		VectorMap<String, T> map;
};

}

#endif
