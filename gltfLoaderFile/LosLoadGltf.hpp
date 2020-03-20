/*
   Create Loskutnikov Sergei 20.11.2017   to load gltf file's 
*/


#pragma once


#include <cassert>
#include <cstring>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>  // assert.h

#include "../gltfLoaderFile/picojson.h"



namespace LosGltf {




#define TINYGLTF_TYPE_VEC2 (2)
#define TINYGLTF_TYPE_VEC3 (3)
#define TINYGLTF_TYPE_VEC4 (4)
#define TINYGLTF_TYPE_MAT2 (32 + 2)
#define TINYGLTF_TYPE_MAT3 (32 + 3)
#define TINYGLTF_TYPE_MAT4 (32 + 4)
#define TINYGLTF_TYPE_SCALAR (64 + 1)
#define TINYGLTF_TYPE_VECTOR (64 + 4)
#define TINYGLTF_TYPE_MATRIX (64 + 16)

#define TINYGLTF_TARGET_ARRAY_BUFFER (34962)
#define TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER (34963)
  

  #define TINYGLTF_MODE_POINTS (0)
#define TINYGLTF_MODE_LINE (1)
#define TINYGLTF_MODE_LINE_LOOP (2)
#define TINYGLTF_MODE_TRIANGLES (4)
#define TINYGLTF_MODE_TRIANGLE_STRIP (5)
#define TINYGLTF_MODE_TRIANGLE_FAN (6)

#define TINYGLTF_COMPONENT_TYPE_BYTE (5120)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE (5121)
#define TINYGLTF_COMPONENT_TYPE_SHORT (5122)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT (5123)
#define TINYGLTF_COMPONENT_TYPE_INT (5124)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT (5125)
#define TINYGLTF_COMPONENT_TYPE_FLOAT (5126)
#define TINYGLTF_COMPONENT_TYPE_DOUBLE (5127)




typedef struct 
{
  
 int bufferView;  // optional in spec but required here since sparse accessor
                   // are not supported
  std::string name;
  size_t byteOffset;
  bool normalized;    // optinal.
  int componentType;  // (required) One of TINYGLTF_COMPONENT_TYPE_***
  size_t count;       // required
  int type;           // (required) One of TINYGLTF_TYPE_***   ..
  //Value extras;

  std::vector<double> minValues;  // optional
  std::vector<double> maxValues; 


}AccessorLos;




struct PrimitiveLos {
  std::map<std::string, int> attributes;  // (required) A dictionary object of
                                          // integer, where each integer
                                          // is the index of the accessor
                                          // containing an attribute.
  int material;  // The index of the material to apply to this primitive
                 // when rendering.
  int indices;   // The index of the accessor that contains the indices.
  int mode;      // one of TINYGLTF_MODE_***
  std::vector<std::map<std::string, int> > targets;  // array of morph targets,
  // where each target is a dict with attribues in ["POSITION, "NORMAL",
  // "TANGENT"] pointing
  // to their corresponding accessors
 // Value extras;

  PrimitiveLos() {
    material = -1;
    indices = -1;
  }
};





typedef struct {
  std::string name;
  std::vector<PrimitiveLos> primitives;
  std::vector<double> weights;  // weights to be applied to the Morph Targets
  std::vector<std::map<std::string, int> > targets;
  //ParameterMap extensions;
 // Value extras;
} MeshLos;



class NodeLos {
 public:
 // was  Node() : camera(-1), skin(-1), mesh(-1) {}
NodeLos() : mesh(-1) {}
  ~NodeLos() = default;

  int camera;  // the index of the camera referenced by this node

  std::string name;
  int skin;
  int mesh;
  std::vector<int> children;
  std::vector<double> rotation;     // length must be 0 or 4
  std::vector<double> scale;        // length must be 0 or 3
  std::vector<double> translation;  // length must be 0 or 3
  std::vector<double> matrix;       // length must be 0 or 16
  std::vector<double> weights;  // The weights of the instantiated Morph Target

  //Value extras;
};


 

   class GLTFlos 
   {

   public: 

   	 GLTFlos() : bin_data_(nullptr) { }

    bool LoadASCIIFromFile(const std::string &filename, const std::string& nameF);
    bool LoadASCIIFromString(const char *buff, const unsigned int length, const std::string& biname, const std::string& namefile);
    bool LoadFromString(const char *str, unsigned int length, const std::string &base_dir, const std::string& filename);


   std::string binFile;
   const unsigned char *bin_data_;
  size_t bin_size_;
  bool is_binary_;
  char pad[7];

   }; // end's class 

}  // end's namespace 




#include <algorithm>
#include <fstream>
#include <sstream>

								// was std::vector<unsigned char> *out
static bool LoadExternalFile(std::vector<unsigned char> *out, const std::string &uri,
                             const std::string &basedir, size_t reqBytes,
                             bool checkSize) {
  //out->clear();

  std::vector<std::string> paths;
  paths.emplace_back(basedir); // was push_back
  paths.emplace_back(".");
  
 printf(" in this load file \n");

  //  std::string filepath
 const auto filepath= basedir; // = FindFile(paths, filename);
 

  if (filepath.empty() || filepath.empty()) {
    
      printf("File not found : %s ", filepath.c_str());
    
    return false;
  }

  std::ifstream f(filepath.c_str(), std::ifstream::binary);
  if (!f) {
    
      printf("File open error : %s \n",filepath.c_str());
    
    return false;
  }

  f.seekg(0, f.end);
  size_t sz = static_cast<size_t>(f.tellg());
  if (int(sz) < 0) {
    std::cout << " size_t less zero " << '\n';
    return false;
  }

  if (sz == 0) {
   std::cout << " Invalid file size  " << '\n';
    return false;
  }

  std::cout << " size_t file's == " <<  sz <<'\n';
  std::vector<unsigned char> buf(sz);

  f.seekg(0, f.beg);
  f.read(reinterpret_cast<char *>(&buf.at(0)),
         static_cast<std::streamsize>(sz));
  f.close();

  if (checkSize) {
    if (reqBytes == sz) {
      out->swap(buf);
      return true;
    } else {
     
      printf("File size mismatch : \n");
         
      return false;
    }
  }

  out->swap(buf);
  return true;
}



static bool ParseBooleanProperty(bool *ret,
                                 const picojson::object &o,
                                 const std::string &property,
                                 const bool required,
                                 const std::string &parent_node = "") {
  auto it = o.find(property);
  if (it == o.end()) {
    if (required) {
        printf( "' property is missing") ;
        if (!parent_node.empty()) {
          printf(" in  empty node ! \n");
        }
    }
    return false;
  }

  if (!it->second.is<bool>()) {
    if (required) {
        printf("' property is not a bool type.\n");
    }
    return false;
  }

  if (ret) {
    (*ret) = it->second.get<bool>();
  }

  return true;
}


static bool ParseNumberArrayProperty(std::vector<double> *ret,
                                     const picojson::object &o,
                                     const std::string &property, bool required,
                                     const std::string &parent_node = "") {
  auto it = o.find(property);
  if (it == o.end()) {
    if (required) {
        printf( "' property is missing");
        if (!parent_node.empty()) {
          printf(" in empty 0 1 \n ");
        }
    }
    return false;
  }

  if (!it->second.is<picojson::array>()) {
    if (required) {

        printf("' property is not an array");
        if (!parent_node.empty()) {
          printf(" in  empty  easy \n"); 
        }
    } 
    return false;
  }

  ret->clear();
  const picojson::array &arr = it->second.get<picojson::array>();
  for (size_t i = 0; i < arr.size(); i++) {
    if (!arr[i].is<double>()) {
      if (required) {
        
          printf("' property is not a number.\n");
          if (!parent_node.empty()) {
             printf(" in node_empty() \n"); 
          }
          
      }
      return false;
    }
    ret->push_back(arr[i].get<double>());
  }

  return true;
}

static bool ParseNumberProperty(double *ret,
                                const picojson::object &o,
                                const std::string &property,
                                const bool required,
                                const std::string &parent_node = "")
{

  auto it = o.find(property);
  if (it == o.end()) {
    if (required) {
      
        printf("' property is missing");
        if (!parent_node.empty()) {
          printf(" in parent Node \n");
        }
        //(*err) += ".\n";
      
    }
    return false;
  }

  if (!it->second.is<double>()) {
    if (required) {
      
         printf("' property is not a number type.\n");
      
    }
    return false;
  }

  if (ret) {
    (*ret) = it->second.get<double>();
  }

	return true;
}


static bool ParseStringProperty(
    std::string *ret, const picojson::object &o,
    const std::string &property, bool required,
    const std::string &parent_node = std::string())
{

auto it = o.find(property);
  if (it == o.end()) {
    if (required) {
          printf("' property is missing");
        if (parent_node.empty()) {
          printf(" parent Node empty .\n");
        } else {
          printf(" else parent node empty ");
        }
     
    }
    return false;
  }

  if (!it->second.is<std::string>()) {
    if (required) {
     printf("' property is not a string type.\n");
      }

    return false;
  }

  if (ret) {
    (*ret) = it->second.get<std::string>();
  }

	return true;
}


static bool ParseStringIntProperty(std::map<std::string, int> *ret,
                                   const picojson::object &o,
                                   const std::string &property, bool required) {
  auto it = o.find(property);
  if (it == o.end()) {
    if (required) {
        printf("' property is missing.\n");
    }
    return false;
  }

  // Make sure we are dealing with an object / dictionary.
  if (!it->second.is<picojson::object>()) {
    if (required) {
        printf("' property is not an object.\n");
    }
    return false;
  }

  ret->clear();
  const picojson::object &dict = it->second.get<picojson::object>();

  auto dictIt(dict.begin());
  auto dictItEnd(dict.end());

  for (; dictIt != dictItEnd; ++dictIt) {
    if (!dictIt->second.is<double>()) {
      if (required) {
        
          printf("' value is not an int.\n");
        
      }
      return false;
    }

    // Insert into the list.
    (*ret)[dictIt->first] = static_cast<int>(dictIt->second.get<double>());
  }
  return true;
}



static bool ParseBuffer(const picojson::object &o, const std::string &basedir,
                        bool is_binary = false,
                        const unsigned char *bin_data = nullptr,
                        size_t bin_size = 0)
{
 
    printf(" ParseBuffer ! \n");
      

 double byteLength;
  if (!ParseNumberProperty(&byteLength, o, "byteLength", true, "Buffer")) {
    return false;
  }

  printf(" in buffers 1 \n");

  // In glTF 2.0, uri is not mandatory anymore
  std::string uri;
  ParseStringProperty(&uri, o, "uri", false, "Buffer");
 
  printf(" my uri name == %s \n", uri.c_str());

  // having an empty uri for a non embedded image should not be valid
  if (!is_binary && uri.empty()) {
    
      printf("'uri' is missing from non binary glTF file buffer.\n");
  }
  
 printf(" in buffer 2 \n");
   auto type = o.find("type");
  if (type != o.end()) {
    if (type->second.is<std::string>()) {
      const std::string &ty = (type->second).get<std::string>();
      if (ty.compare("arraybuffer") == 0) {
        // buffer.type = "arraybuffer";
      }
    }
  }

//std::vector<unsigned char> *out
 std::vector<unsigned char> data;

 printf(" in buffer 3 \n");
  auto bytes = static_cast<size_t>(byteLength);
  if (is_binary) {
    // Still binary glTF accepts external dataURI. First try external resources.

    if (!uri.empty()) {
      // External .bin file.
      LoadExternalFile(&data, uri, basedir, bytes, true);
      printf(" uri not empty \n" );
    } else {
      // load data from (embedded) binary data

      if ((bin_size == 0) || (bin_data == nullptr)) {
      
          printf("Invalid binary data in `Buffer'.\n");
        
        return false;
      }
      else 
      {
      	printf(" this \n");
      	return false;
      }
   }
}
else 
{
	 printf(" not binary \n");
}



printf(" byteLengt == %f \n", byteLength);
      // Read buffer data
      //buffer->data.resize(static_cast<size_t>(byteLength));
      //memcpy(&(buffer->data.at(0)), bin_data, static_cast<size_t>(byteLength));
 
    // for(unsigned char& ji : data)
    // {
    // 	printf(" %c  \n", ji);
    // }
 
//  data.resize(static_cast<size_t>(byteLength));
 // memcpy(&(data.at(0)), bin_data, static_cast<size_t>(byteLength));

 // my data 
  
   // Example Work correct 
      //   const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
      // GLBufferState state;
      // glGenBuffers(1, &state.vb);
      // glBindBuffer(bufferView.target, state.vb);
      // std::cout << "buffer.size= " << buffer.data.size()
      //           << ", byteOffset = " << bufferView.byteOffset << std::endl;
      // glBufferData(bufferView.target, bufferView.byteLength,
      //              &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
 
  
      //  "bufferView": 0,
      // "byteOffset": 0,
      // "componentType": 5123,
      // "count": 44529,
      // "max": [ 9802 ],
      // "min": [ 0 ],
      // "type": "SCALAR"

// "buffer": 0,
//       "byteLength": 347802,
//       "byteOffset": 0,
//       "byteStride": 0,
//       "target": 34963
  
 

   std::cout << " my lenght data = " << data.size() << '\n';

    

    std::vector <float> dataFloat;

      dataFloat.resize(static_cast<size_t>(347802));
  memcpy(&(dataFloat.at(0)), data.data(), static_cast<size_t>(347802));
 

    std::cout << " my size dataFloat = " << dataFloat.size() << '\n';
    

      for (int i = 0; i < 20; ++i)
      { 
       // std::cout << " my i = "<< i << " & " <<  data.at(i) << '\n';
        printf(" my data == %u \n", dataFloat.at(i));
      }

   std::string nameLosParse;

	ParseStringProperty(&nameLosParse, o, "name", false);
  

   printf(" nameLosParse  == %s \n", nameLosParse.c_str());


	return true;
}

						// was BufferView *bufferView,
static bool ParseBufferView(const picojson::object &o)
{


double buffer = -1.0; // was &buffer, err,
  if (!ParseNumberProperty(&buffer, o, "buffer", true, "BufferView")) { 
    return false;
  }

  double byteOffset = 0.0;
  ParseNumberProperty(&byteOffset, o, "byteOffset", false);
  std::cout << " Pbw byteOffset  = " << byteOffset << '\n';


  double byteLength = 1.0;
  if (!ParseNumberProperty(&byteLength, o, "byteLength", true,
                           "BufferView")) {
    return false;
  }
 
  std::cout << " Pbw byteLength  = " << byteLength << '\n';
 

  size_t byteStride = 0;
  double byteStrideValue = 0.0;
  if (!ParseNumberProperty(&byteStrideValue, o, "byteStride", false)) {
    // Spec says: When byteStride of referenced bufferView is not defined, it
    // means that accessor elements are tightly packed, i.e., effective stride
    // equals the size of the element.
    // We cannot determine the actual byteStride until Accessor are parsed, thus
    // set 0(= tightly packed) here(as done in OpenGL's VertexAttribPoiner)
    byteStride = 0;
  } else {
    byteStride = static_cast<size_t>(byteStrideValue);
  }

  std::cout << " Pbw byteStride  = " << byteStride << '\n';

  if ((byteStride > 252) || ((byteStride % 4) != 0)) {
    printf("Invalid `byteStride' value. `byteStride' must be the multiple of ");
    return false;
  }

  double target = 0.0;
  ParseNumberProperty(&target, o, "target", false);
  int targetValue = static_cast<int>(target);
  if ((targetValue == TINYGLTF_TARGET_ARRAY_BUFFER) ||
      (targetValue == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER)) {
    // OK
  } else {
    targetValue = 0;
  }

 
  printf(" parse bufferView ! \n");

  // bufferView->target = targetValue;

  // ParseStringProperty(&bufferView->name, err, o, "name", false);

  // bufferView->buffer = static_cast<int>(buffer);
  // bufferView->byteOffset = static_cast<size_t>(byteOffset);
  // bufferView->byteLength = static_cast<size_t>(byteLength);
  // bufferView->byteStride = static_cast<size_t>(byteStride);




return true;
}

				// was Accessor *accessor,
static bool ParseAccessor(LosGltf::AccessorLos* accessor, const picojson::object &o)
{

 std::cout << " Parse accessor ! " << '\n';
  double bufferView = -1.0;
  if (!ParseNumberProperty(&bufferView, o, "bufferView", true,
                           "Accessor")) {
    return false;
  }

  double byteOffset = 0.0;
  ParseNumberProperty(&byteOffset, o, "byteOffset", false, "Accessor");
 
 std::cout << "  byteOffset == "<< byteOffset << '\n';

  bool normalized = false;
  ParseBooleanProperty(&normalized, o, "normalized", false, "Accessor");

  double componentType = 0.0;
  if (!ParseNumberProperty(&componentType, o, "componentType", true,
                           "Accessor")) {
    return false;
  }

  double count = 0.0;
  if (!ParseNumberProperty(&count, o, "count", true, "Accessor")) {
    return false;
  }

  std::string type;
  if (!ParseStringProperty(&type, o, "type", true, "Accessor")) {
    return false;
  }

  if (type.compare("SCALAR") == 0) {
    accessor->type = TINYGLTF_TYPE_SCALAR;
  } else if (type.compare("VEC2") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC2;
  } else if (type.compare("VEC3") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC3;
  } else if (type.compare("VEC4") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC4;
  } else if (type.compare("MAT2") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT2;
  } else if (type.compare("MAT3") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT3;
  } else if (type.compare("MAT4") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT4;
  } else {
    
     printf("Unsupported `type` for accessor object. Got \" \n");

    return false;
  }

  ParseStringProperty(&accessor->name, o, "name", false);

  accessor->minValues.clear();
  accessor->maxValues.clear();
  ParseNumberArrayProperty(&accessor->minValues, o, "min", false,
                           "Accessor");

  ParseNumberArrayProperty(&accessor->maxValues, o, "max", false,
                           "Accessor");

  accessor->count = static_cast<size_t>(count);
  accessor->bufferView = static_cast<int>(bufferView);
  accessor->byteOffset = static_cast<size_t>(byteOffset);
  accessor->normalized = normalized;
  {
    int comp = static_cast<int>(componentType);
    if (comp >= TINYGLTF_COMPONENT_TYPE_BYTE &&
        comp <= TINYGLTF_COMPONENT_TYPE_DOUBLE) {
      // OK
      accessor->componentType = comp;
    } else {
      
      printf("Invalid `componentType` in accessor. Got \n");
   
      return false;
    }
  }

  //ParseExtrasProperty(&(accessor->extras), o);

	return true;
}

	
				// was Primitive *primitive,
static bool ParsePrimitive(LosGltf::PrimitiveLos* primitive, const picojson::object &o) 
{
 
 std::cout << " Parse Primitive " << '\n';
   double material = -1.0;
  ParseNumberProperty(&material, o, "material", false);
  primitive->material = static_cast<int>(material);

  double mode = static_cast<double>(TINYGLTF_MODE_TRIANGLES);
  ParseNumberProperty(&mode, o, "mode", false);

  int primMode = static_cast<int>(mode);
  primitive->mode = primMode;  // Why only triangled were supported ?

  double indices = -1.0;
  ParseNumberProperty(&indices, o, "indices", false);
  primitive->indices = static_cast<int>(indices);
  if (!ParseStringIntProperty(&primitive->attributes, o, "attributes",
                              true)) {
    return false;
  }

  // Look for morph targets
  picojson::object::const_iterator targetsObject = o.find("targets");
  if ((targetsObject != o.end()) &&
      (targetsObject->second).is<picojson::array>()) {
    const picojson::array &targetArray =
        (targetsObject->second).get<picojson::array>();
    for (size_t i = 0; i < targetArray.size(); i++) {
      std::map<std::string, int> targetAttribues;

      const picojson::object &dict = targetArray[i].get<picojson::object>();
      picojson::object::const_iterator dictIt(dict.begin());
      picojson::object::const_iterator dictItEnd(dict.end());

      for (; dictIt != dictItEnd; ++dictIt) {
        targetAttribues[dictIt->first] =
            static_cast<int>(dictIt->second.get<double>());
      }
      primitive->targets.push_back(targetAttribues);
    }
  }

  //ParseExtrasProperty(&(primitive->extras), o);

	return true;
}



			// was  Mesh *mesh
static bool ParseMesh(LosGltf::MeshLos* mesh, const picojson::object &o)
{
  
  std::cout << " Parse Mesh " << '\n';
ParseStringProperty(&mesh->name, o, "name", false);

  mesh->primitives.clear();
  picojson::object::const_iterator primObject = o.find("primitives");
  if ((primObject != o.end()) && (primObject->second).is<picojson::array>()) {
    const picojson::array &primArray =
        (primObject->second).get<picojson::array>();
    for (size_t i = 0; i < primArray.size(); i++) {
      LosGltf::PrimitiveLos primitive;
      if (ParsePrimitive(&primitive,
                         primArray[i].get<picojson::object>())) {
        // Only add the primitive if the parsing succeeds.
        mesh->primitives.push_back(primitive);
      }
    }
  }

  // Look for morph targets
  picojson::object::const_iterator targetsObject = o.find("targets");
  if ((targetsObject != o.end()) &&
      (targetsObject->second).is<picojson::array>()) {
    const picojson::array &targetArray =
        (targetsObject->second).get<picojson::array>();
    for (size_t i = 0; i < targetArray.size(); i++) {
      std::map<std::string, int> targetAttribues;

      const picojson::object &dict = targetArray[i].get<picojson::object>();
      picojson::object::const_iterator dictIt(dict.begin());
      picojson::object::const_iterator dictItEnd(dict.end());

      for (; dictIt != dictItEnd; ++dictIt) {
        targetAttribues[dictIt->first] =
            static_cast<int>(dictIt->second.get<double>());
      }
      mesh->targets.push_back(targetAttribues);
    }
  }

  // Should probably check if has targets and if dimensions fit
  ParseNumberArrayProperty(&mesh->weights, o, "weights", false);

  //ParseExtrasProperty(&(mesh->extras), o);

	return true;
}


				// was  Node *node
static bool ParseNode(LosGltf::NodeLos *node, const picojson::object &o)
{

std::cout << " Parse Node " << '\n';
ParseStringProperty(&node->name, o, "name", false);

  double skin = -1.0;
  ParseNumberProperty(&skin, o, "skin", false);
  node->skin = static_cast<int>(skin);

  // Matrix and T/R/S are exclusive
  // if (!ParseNumberArrayProperty(&node->matrix, o, "matrix", false)) {
  //   ParseNumberArrayProperty(&node->rotation, o, "rotation", false);
  //   ParseNumberArrayProperty(&node->scale, o, "scale", false);
  //   ParseNumberArrayProperty(&node->translation, o, "translation", false);
  // }

  // double camera = -1.0;
  // ParseNumberProperty(&camera, o, "camera", false);
  // node->camera = static_cast<int>(camera);

  double mesh = -1.0;
  ParseNumberProperty(&mesh, o, "mesh", false);
  node->mesh = int(mesh);

  node->children.clear();
  picojson::object::const_iterator childrenObject = o.find("children");
  if ((childrenObject != o.end()) &&
      (childrenObject->second).is<picojson::array>()) {
    const picojson::array &childrenArray =
        (childrenObject->second).get<picojson::array>();
    for (size_t i = 0; i < childrenArray.size(); i++) {
      if (!childrenArray[i].is<double>()) {
      
          printf("Invalid `children` array.\n");
        
        return false;
      }
      const int &childrenNode =
          static_cast<int>(childrenArray[i].get<double>());
      node->children.push_back(childrenNode);
    }
  }

 // ParseExtrasProperty(&(node->extras), o);

	return true;
}



















namespace LosGltf
{



bool GLTFlos::LoadASCIIFromString(const char *buff, const unsigned int length, const std::string& biname, const std::string& namefile)
{
 
  is_binary_ = false;
  bin_data_ = NULL;
  bin_size_ = 0;

  return LoadFromString(buff, length, biname, namefile);
}

 bool GLTFlos::LoadASCIIFromFile(const std::string &filename, const std::string& nameF)
{
 
     std::cout << " in the strtakjhf " << filename << '\n';
    std::cout  << " my file  " << nameF << '\n';
   
 

  std::string str = filename;
  std::string anotherStr = filename;
  std::string str2= nameF;
 //  std::string str3="print 10 and then 5 more";

 //  // used in the same order as described above:
   str.append(str2);
 
  // str == path + name File  '.gltf'
  

  const auto jio = str2.find_last_of('.');
  std::string toKi = str2.substr(0, jio);
  printf(" my string To in == %s \n", toKi.c_str()); 
 std::string bn = ".bin";
 toKi.append(bn);
 
  printf(" file .bin = %s \n", toKi.c_str());
  std::cout << " full path to .bin " << anotherStr.append(toKi) << '\n';


    std::stringstream ss;

  // to load .gltf file's 
  std::ifstream f(str.c_str());
  if (!f) {
    ss << "Failed to open file: " << filename << '\n';
    // if (err) {
    //   (*err) = ss.str();
    // }
    return false;
  }

  f.seekg(0, f.end);
  size_t sz = static_cast<size_t>(f.tellg());
  std::vector<char> buf(sz);

  if (sz == 0) {
    // if (err) {
    //   (*err) = "Empty file.";
    // }
    return false;
  }

  f.seekg(0, f.beg);
  f.read(&buf.at(0), static_cast<std::streamsize>(sz));
  f.close();

  // std::string basedir = GetBaseDir(filename);

  bool ret = LoadASCIIFromString( &buf.at(0),
                                 static_cast<unsigned int>(buf.size()), anotherStr, toKi);



 
  return ret;
}



// #ifdef __clang__
// #pragma clang diagnostic pop
// #endif





bool GLTFlos::LoadFromString(const char *str, unsigned int length, const std::string &base_dir, const std::string& filename ) {

    printf(" we are the stringFromLoad !! and basedir = %s \n", base_dir.c_str());


     if (length < 4) {
    
       printf("JSON string too short.\n");
    
    return false;
  }

  // TODO(syoyo): Add feature not using exception handling.
  picojson::value v;
  try {
    std::string perr = picojson::parse(v, str, str + length);

    if (!perr.empty()) {
         printf("JSON parsing error: %s ", perr.c_str());
      return false;
    }
  } catch (std::exception e) {
    
     printf("  Error == %s \n", e.what());
    return false;
  }

  if (!v.is<picojson::object>()) {
    // root is not an object.
    
    printf( "Root element is not a JSON object\n");
    
    return false;
  }


 if (v.contains("scenes") && v.get("scenes").is<picojson::array>()) {
    // OK
  } else {
    
      printf("\"scenes\" object not found in .gltf\n");
    
    return false;
  }

  if (v.contains("nodes") && v.get("nodes").is<picojson::array>()) {
    // OK
  } else {
    
     printf("\"nodes\" object not found in .gltf\n");
    
    return false;
  }

  if (v.contains("accessors") && v.get("accessors").is<picojson::array>()) {
    // OK
  } else  {
    
      printf("\"accessors\" object not found in .gltf\n");
    
    return false;
  }

  if (v.contains("buffers") && v.get("buffers").is<picojson::array>()) {
    // OK
  } else {
   
      printf("\"buffers\" object not found in .gltf\n");
    return false;
  }
 

    if (v.contains("bufferViews") && v.get("bufferViews").is<picojson::array>()) {
    // OK
  } else {
    
      printf( "\"bufferViews\" object not found in .gltf\n");
    
    return false;
  }

  

     if (v.contains("buffers") && v.get("buffers").is<picojson::array>()) {
    const picojson::array &root = v.get("buffers").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!it->is<picojson::object>()) {
        
          printf("`buffers' does not contain an JSON object.");
        return false;
      }

 printf(" in the buffer 's \n");

  is_binary_ = true;
      // Buffer buffer;
      if (!ParseBuffer(it->get<picojson::object>(), base_dir,
                       is_binary_, bin_data_, bin_size_)) {
        return false;
      }

      // model->buffers.push_back(buffer);
    }
  }



  // 2. Parse BufferView
  if (v.contains("bufferViews") && v.get("bufferViews").is<picojson::array>()) {
    const picojson::array &root = v.get("bufferViews").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!it->is<picojson::object>()) {
        
          printf( "`bufferViews' does not contain an JSON object.");
        
        return false;
      }
    
      // BufferView bufferView;
      if (!ParseBufferView( it->get<picojson::object>())) {
        return false;
      }

      // model->bufferViews.push_back(bufferView);
    }

  }

  AccessorLos accessorOne;

  // 3. Parse Accessor
  if (v.contains("accessors") && v.get("accessors").is<picojson::array>()) {
    const picojson::array &root = v.get("accessors").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!it->is<picojson::object>()) {
       
          printf("`accessors' does not contain an JSON object.");
        
        return false;
      }
      // Accessor accessor;
      if (!ParseAccessor(&accessorOne, it->get<picojson::object>())) {
        return false;
      }

      // model->accessors.push_back(accessor);
    }
  }

  // 4. Parse Mesh
  if (v.contains("meshes") && v.get("meshes").is<picojson::array>()) {
    const picojson::array &root = v.get("meshes").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!it->is<picojson::object>()) {
       
          printf( "`meshes' does not contain an JSON object.");
        
        return false;
      }
      MeshLos mesh;
      if (!ParseMesh(&mesh, it->get<picojson::object>())) {
        return false;
      }

      // model->meshes.push_back(mesh);
    }
  }

  // 5. Parse Node
  if (v.contains("nodes") && v.get("nodes").is<picojson::array>()) {
    const picojson::array &root = v.get("nodes").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!it->is<picojson::object>()) {
          printf( "`nodes' does not contain an JSON object.");
        return false;
      }
      NodeLos node;
      if (!ParseNode(&node, it->get<picojson::object>())) {
        return false;
      }

      // model->nodes.push_back(node);
    }
  }

  // 6. Parse scenes.
  if (v.contains("scenes") && v.get("scenes").is<picojson::array>()) {
    const picojson::array &root = v.get("scenes").get<picojson::array>();

    picojson::array::const_iterator it(root.begin());
    picojson::array::const_iterator itEnd(root.end());
    for (; it != itEnd; it++) {
      if (!(it->is<picojson::object>())) {
       
          printf("`scenes' does not contain an JSON object.");
        return false;
      }
      const picojson::object &o = it->get<picojson::object>();
      std::vector<double> nodes;
      if (!ParseNumberArrayProperty(&nodes, o, "nodes", false)) {
        return false;
      }

      // Scene scene;
      // ParseStringProperty(&scene.name, o, "name", false);
      // std::vector<int> nodesIds;
      // for (size_t i = 0; i < nodes.size(); i++) {
      //   nodesIds.push_back(static_cast<int>(nodes[i]));
      // }
      // scene.nodes = nodesIds;

      // model->scenes.push_back(scene);
    }
  }

  // 7. Parse default scenes.
  if (v.contains("scene") && v.get("scene").is<double>()) {
    const int defaultScene = int(v.get("scene").get<double>());

    //model->defaultScene = static_cast<int>(defaultScene);
  }
 








    return true;

 }

}








void LoadASCIFLos(const std::string& filename, const std::string& path)
{

   LosGltf::GLTFlos  gltfLos;
   bool ret;

   std::cout << filename << '\n';
   std::cout << path << '\n';
  
  
     auto const newqwLd = path.find_first_of('.');
     std::cout << " number to == " <<  newqwLd << '\n';
     std::string hui = path.substr(0, newqwLd); // smile 

       printf(" find_first_of = %s \n", hui.c_str());


   //filename.append(path);

    std::cout << "Reading ASCII  Los  glTF" << '\n';
    // assume ascii glTF.
    ret = gltfLos.LoadASCIIFromFile(filename.c_str(), path);


}









