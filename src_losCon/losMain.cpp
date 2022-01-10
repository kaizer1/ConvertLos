/*
* Loskutnikov Convertor 2017 
*
 */

#include <string>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string>

#include "fbx_common.h"
 
#include <read_struct.h>
//#include <incl/example_c.h>

#include <fstream>

//#include <picojson.h>
#include <LosLoadGltf.hpp>
#include <unordered_map>

const int TRIANGLE_VERTEX_STRIDE = 3;
const int VERTEX_STRIDE = 4;
const int NORMAL_STRIDE = 3;
const int UV_STRIDE = 2;
#define TRIANGLE_VERTEX_COUNT 3


// sdlkfjasjkldjflkasdjfas
// asdklhfkjasdhfkjasdhfjk
typedef std::unordered_map<const FbxNode*, unsigned int> NodeToBoneMap;


// branck workLast

   // struct SubMesh
   //  {
   //      SubMesh() : IndexOffset(0), TriangleCount(0) {}

   //      int IndexOffset;
   //      int TriangleCount;
   //  };

struct FHeader
{
    char signature;
    int version;
};
 
struct FData
{
    int number;
    char sign;
    bool state;
    //float [] todo;
};

struct LosVecFour
{
   float l1;
   float l2;
   float l3;
};
 
struct FContainer
{
    FHeader header;
    FData data;
    LosVecFour vectLos[2];
};


struct Deve 
{
  float serv[3];
  float roi[3];
};




bool AnimTrue = false;


 

 struct LosBestMesh
 {
   

     LosBestMesh() 
     {
    vertexPolygonCount = 0; 
    PolygonCount = 0;
    normalCount = 0;
    uvCount = 0;
    tangentCount = 0;
    biNormalCount = 0;

    existsBiNorm = false;
    existsTang  = false;
     }



 const char *losString = nullptr;

   int vertexPolygonCount; 
   int PolygonCount;
   int normalCount;
   int uvCount;
   int tangentCount;
   int biNormalCount;

   float *lmVertices = nullptr;
   unsigned int *lmIndices = nullptr; 
   float *lmNormal  = nullptr;
   float *lmUV = nullptr;
   float *lmTangents = nullptr; 
   float *lmBiNormal = nullptr;

   int *serExal{};
   //float hitler[];

   bool existsBiNorm; 
   bool existsTang;
   
 };



class SkinBinding {
 public:
  typedef uint16_t BoneIndex;
  typedef uint8_t PackedBoneIndex;
  typedef uint8_t PackedWeight;
  static const unsigned int kInfluenceMax = 4;
  static const BoneIndex kNoBoneIndex = 0xffff;
  static const BoneIndex kBoneIndexMax = 0xfffe;
  static const PackedBoneIndex kPackedBoneIndexMax = 0xff;
  static const PackedWeight kPackedWeightOne = 0xff;

  SkinBinding() { Clear(); }

  [[nodiscard]]const BoneIndex* GetBoneIndices() const { return bone_indices_; }
  [[nodiscard]]const float* GetBoneWeights() const { return bone_weights_; }

  void Clear() {
    for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
         ++influence_index) {
      bone_indices_[influence_index] = kNoBoneIndex;
    }
    for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
         ++influence_index) {
      bone_weights_[influence_index] = 0.0f;
    }
  }

  [[nodiscard]] bool HasInfluences() const { return bone_indices_[0] != kNoBoneIndex; }

  [[nodiscard]] unsigned int CountInfluences() const {
    for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
         ++influence_index) {
      if (bone_indices_[influence_index] == kNoBoneIndex)
        return influence_index;
    }
    return kInfluenceMax;
  }

  void AppendInfluence(unsigned int bone_index, float bone_weight, const FbxMesh* log_mesh, unsigned int log_vertex_index) {
    unsigned int influence_count = CountInfluences();
     printf(" influence_count == %d \n", influence_count);
    // Discard the smallest influence if we reach capacity.
    if (influence_count == kInfluenceMax) {
      const unsigned int smallest_influence_index =
          FindSmallestInfluence(influence_count);
      const float smallest_bone_weight =
          bone_weights_[smallest_influence_index];
      if (smallest_bone_weight < bone_weight) {
        // Existing influence is the smallest.
        const BoneIndex smallest_bone_index =
            bone_indices_[smallest_influence_index];
        EraseInfluence(influence_count, smallest_influence_index);
        --influence_count;
        // printf("Too many skin influences (max=%u) for mesh %s vertex %u."
        //         " Discarding the smallest influence (%f) to bone %u.\n",
        //         kInfluenceMax, GetMeshOrNodeName(log_mesh), log_vertex_index,
        //         smallest_bone_weight, smallest_bone_index);

         printf(" Mistakle int sdofas \n");
      } else {
        // New influence is the smallest.
           // printf("Too many skin influences (max=%u) for mesh %s vertex %u."
           //      " Discarding the smallest influence (%f) to bone %u.\n",
           //      kInfluenceMax, GetMeshOrNodeName(log_mesh), log_vertex_index,
           //      bone_weight, bone_index);
        printf("  Ykajsdhfjk \n" );
        return;
      }
    }
     printf("  Pre asserts \n");
    // Append the influence.
    assert(bone_index <= kBoneIndexMax);
    printf("  Post asserts \n");
    bone_indices_[influence_count] = static_cast<BoneIndex>(bone_index);
    bone_weights_[influence_count] = bone_weight;
  }

  // Set the vertex to single-bone rigid binding.
  void BindRigid(BoneIndex bone_index) {
    Clear();
    bone_indices_[0] = bone_index;
    bone_weights_[0] = 1.0f;
  }

  // Normalize weights to sum to 1.0.
  void NormalizeBoneWeights() {
    unsigned int influence_count = 0;
    float bone_weight_sum = 0.0f;
    for (; influence_count != kInfluenceMax; ++influence_count) {
      if (bone_indices_[influence_count] == kNoBoneIndex) break;
      bone_weight_sum += bone_weights_[influence_count];
    }

    if (influence_count == 0) {
      // Vertex not weighted to any bone.  Set full weighting to the origin.
      bone_weights_[0] = 1.0f;
    } else if (bone_weight_sum == 0.0f) {
      // Weights sum to 0.  Probably shouldn't happen, but if it does just
      // evenly distribute weights.
      const float bone_weight = 1.0f / static_cast<float>(influence_count);
      for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
           ++influence_index) {
        bone_weights_[influence_index] = bone_weight;
      }
    } else {
      // Scale weights so they sum to 1.0.
      const float scale = 1.0f / bone_weight_sum;
      for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
           ++influence_index) {
        bone_weights_[influence_index] *= scale;
      }
    }
  }

  // Pack indices and weights to 8-bit components, remapping indices with
  // src_to_dst_index_map.
  void Pack(const BoneIndex* src_to_dst_index_map, size_t src_bone_count,
             const char* log_mesh_name,
            unsigned int log_vertex_index
            // Vec4ub* out_packed_indices,
            //Vec4ub* out_packed_weights)
             )
             const {
    PackedBoneIndex packed_indices[4] = {0, 0, 0, 0};
    PackedWeight packed_weights[4] = {0, 0, 0, 0};

    auto src_to_dst_scale = static_cast<float>(kPackedWeightOne);
    unsigned int dst_weight_remain = kPackedWeightOne;
    for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
         ++influence_index) {
      const BoneIndex src_index = bone_indices_[influence_index];
      if (src_index == kNoBoneIndex) {
        break;
      }
      assert(src_index < src_bone_count);

      // This bone is referenced, so it shouldn't have been pruned.
      const BoneIndex dst_index = src_to_dst_index_map[src_index];
      assert(dst_index != kNoBoneIndex);

      if (dst_index > kPackedBoneIndexMax) {
        printf("Bone index %u exceeds %u."
                " Discarding skin weight for mesh %s vertex %u.\n",
                dst_index, kPackedBoneIndexMax, log_mesh_name,
                log_vertex_index);
        break;
      }

      // Pack weight, quantizing from float to byte.  The weight is rounded, and
      // we keep track of the total weight remaining so we can distribute
      // quantization error between weights at the end.
      const float src_weight = bone_weights_[influence_index];
      const float dst_weight = src_weight * src_to_dst_scale;
      const unsigned int dst_weight_rounded = std::min(
          static_cast<unsigned int>(dst_weight + 0.5f), dst_weight_remain);
      dst_weight_remain -= dst_weight_rounded;

      packed_indices[influence_index] = static_cast<PackedBoneIndex>(dst_index);
      packed_weights[influence_index] =
          static_cast<PackedWeight>(dst_weight_rounded);
    }

    // Distribute quantization error between weights, so they sum to 255.
    for (; dst_weight_remain; --dst_weight_remain) {
      // Choose the weight to which adding 1 minimizes error.
      unsigned int best_influence_index = 0;
      float diff_min = FLT_MAX;
      for (unsigned int influence_index = 0; influence_index != kInfluenceMax;
           ++influence_index) {
        if (bone_indices_[influence_index] == kNoBoneIndex) {
          break;
        }
        const float src_weight = bone_weights_[influence_index];
        const float dst_weight =
            static_cast<float>(packed_weights[influence_index] + 1);
        const float diff = dst_weight - src_weight * src_to_dst_scale;
        if (diff < diff_min) {
          best_influence_index = influence_index;
          diff_min = diff;
        }
      }
      packed_weights[best_influence_index] += 1;
    }

//  Los CORRECT ! 
    // *out_packed_indices = Vec4ub(packed_indices[0], packed_indices[1],
    //                              packed_indices[2], packed_indices[3]);
    // *out_packed_weights = Vec4ub(packed_weights[0], packed_weights[1],
    //                              packed_weights[2], packed_weights[3]);
  }

 private:
  BoneIndex bone_indices_[kInfluenceMax];
  float bone_weights_[kInfluenceMax];

  // Find the smallest influence.  If there are multiple smallest influences,
  // this returns the one nearest the end of the array (i.e. most recently
  // added).
  unsigned int FindSmallestInfluence(unsigned int influence_count) const {
    assert(influence_count > 0);
    unsigned int smallest_influence_index = 0;
    for (unsigned int influence_index = 1; influence_index != influence_count;
         ++influence_index) {
      if (bone_weights_[influence_index] <=
          bone_weights_[smallest_influence_index]) {
        smallest_influence_index = influence_index;
      }
    }
    return smallest_influence_index;
  }

  // Erase an influence, preserving the order of the remaining influences.
  void EraseInfluence(unsigned int influence_count,
                      unsigned int influence_index) {
    assert(influence_index < influence_count);
    const unsigned int last_influence_index = influence_count - 1;
    for (; influence_index != last_influence_index; ++influence_index) {
      bone_indices_[influence_index] = bone_indices_[influence_index + 1];
      bone_weights_[influence_index] = bone_weights_[influence_index + 1];
    }
    bone_indices_[last_influence_index] = kNoBoneIndex;
    bone_weights_[last_influence_index] = 0.0f;
  }
};






//static inline vec4 Vec4FromFbx(const FbxVector4& v) {
static inline void Vec4FromFbx(const FbxVector4& v) {
  const FbxDouble* d = v.mData;
   //printf(" vec4FromFbx == %f,  %f,   %f,   %f,  \n", static_cast<float>(d[0]), static_cast<float>(d[1]),
    //                                                  static_cast<float>(d[2]), static_cast<float>(d[3]) );

  // return vec4(static_cast<float>(d[0]), static_cast<float>(d[1]),
  //             static_cast<float>(d[2]), static_cast<float>(d[3]));
}

//static inline vec3 Vec3FromFbx(const FbxVector4& v) {
static inline void Vec3FromFbx(const FbxVector4& v) {
  const FbxDouble* d = v.mData;
 //  printf(" Vec3FromFbx == %f,  %f,  %f, \n", static_cast<float>(d[0]), static_cast<float>(d[1]),
   //                                                   static_cast<float>(d[2]) );
  // return vec3(static_cast<float>(d[0]), static_cast<float>(d[1]),
  //             static_cast<float>(d[2]));
}


// FBX UV format has the v-coordinate inverted from OpenGL.
static inline void Vec2FromFbxUv(const FbxVector2& v) {
  const FbxDouble* d = v.mData;
  //return vec2(static_cast<float>(d[0]), static_cast<float>(1.0 - d[1]));
 // printf(" Vec2FromFbxUv == %f and %f  \n", static_cast<float>(d[0]), static_cast<float>(1.0 - d[1]) );
}





template <class T>
static int ElementDirectIndex(const FbxLayerElementTemplate<T>& element,
                              int index) {
  return element.GetReferenceMode() == FbxGeometryElement::eDirect
             ? index
             : element.GetIndexArray().GetAt(index);
}


template <class T>
static T Element(const FbxLayerElementTemplate<T>& element, int index) {
  const int direct_index = ElementDirectIndex(element, index);
  return element.GetDirectArray().GetAt(direct_index);
}


template <class T>
static T ElementFromIndices(const FbxLayerElementTemplate<T>* element,
                            int control_index, int vertex_counter) {
  if (!element) return T();
  const int index =
      element->GetMappingMode() == FbxGeometryElement::eByControlPoint
          ? control_index
          : vertex_counter;
  return Element(*element, index);
}


struct  newLL
{ 

     private:

   int normalCount;

     public:
       int vertexPolygonCount; 
       float *eleven;

  newLL() {
    vertexPolygonCount = 0;
    normalCount = 1;
    eleven = nullptr;
  } 

};



    struct SubMesh
    {
        SubMesh() : IndexOffset(0), TriangleCount(0) {}
        
        int IndexOffset;
        int TriangleCount;
    };


static const char* g_program_name = nullptr;
FbxArray<SubMesh*> mSubMeshes;
bool hasNormalExists;
bool hasTangentExists;
bool hasUvWxists;
bool hasBiNormalExists;
std::string losNameMesh;
 std::string losOutputMesh;
  
 std::string nameLosMesh = "ExampleMesh";
 std::string nameExampleN_files;



// Bitwise OR of the kVertexAttributeBits.
// typedef uint32_t VertexAttributeBitmask;
// static const VertexAttributeBitmask kVertexAttributeBit_Position =
//     1 << kVertexAttribute_Position;
// static const VertexAttributeBitmask kVertexAttributeBit_Normal =
//     1 << kVertexAttribute_Normal;
// static const VertexAttributeBitmask kVertexAttributeBit_Tangent =
//     1 << kVertexAttribute_Tangent;
// static const VertexAttributeBitmask kVertexAttributeBit_Uv =
//     1 << kVertexAttribute_Uv;
// static const VertexAttributeBitmask kVertexAttributeBit_UvAlt =
//     1 << kVertexAttribute_UvAlt;
// static const VertexAttributeBitmask kVertexAttributeBit_Color =
//     1 << kVertexAttribute_Color;
// static const VertexAttributeBitmask kVertexAttributeBit_Bone =
//     1 << kVertexAttribute_Bone;
// static const VertexAttributeBitmask
//     kVertexAttributeBit_AllAttributesInSourceFile =
//         static_cast<VertexAttributeBitmask>(-1);



// void LoadCacheRecursive(FbxScene* lScene, std::shared_ptr<LosBestMesh> meshL); 
 void LoadCacheRecursive(FbxScene* lScene, LosBestMesh *meshL); 
void LoadCacheRecurThree(FbxNode *pNode, LosBestMesh *meshL);
bool InitializeFbxMesh(const FbxMesh *pMesh, LosBestMesh *meshL);
void PrintTangents(const LosBestMesh *meshTanget);
void PrintBiNormals(const LosBestMesh *meshBiNornals);
void LoadGLTF(std::string& fullOutputPath, std::string& namefbxFile);   
void LoadFBX(std::string& fullOutputPath, std::string& namefbxFile, LosBestMesh *lsMesh);
void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);

bool LosOne(NodeToBoneMap* node_to_bone_map, FbxNode* node);
void LosBoneRecursive(NodeToBoneMap* node_to_bone_map, FbxNode* node, int parent_bone_index);
static int AddBoneForNode(NodeToBoneMap* node_to_bone_map, FbxNode* node, int parent_bone_index);
void LosMesGatherRecursive(const NodeToBoneMap* node_to_bone_map, FbxNode* node, FbxNode* parent_node, FbxScene* scene);
void LosTransforms(FbxNode* node, FbxNode* parent_node, FbxAMatrix* default_bone_transform_inverse, FbxAMatrix* point_transform);
void LosMainGatherFSurface(const FbxMesh* mesh, SkinBinding::BoneIndex transform_bone_index, const NodeToBoneMap* node_to_bone_map,
                         const FbxAMatrix& point_transform);
  void LosGatherSkinBindings(const FbxMesh* mesh,  SkinBinding::BoneIndex transform_bone_index, const NodeToBoneMap* node_to_bone_map,
                          std::vector<SkinBinding>* out_skin_bindings);
 const FbxGeometryElementUV* UvElements( const FbxMesh* mesh, const FbxGeometryElementUV** uv_alt_element);

void LoadGLTF(std::string& fullOutputPath, std::string& namefbxFile)
{

  printf(" load GLTF !! \n");

  LoadASCIFLos(fullOutputPath, namefbxFile);

}



void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
    //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
    pManager = FbxManager::Create();
    if( !pManager )
    {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        exit(1);
    }
  else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

  //Create an IOSettings object. This object holds all import/export settings.
  FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
  pManager->SetIOSettings(ios);

  //Load plugins from the executable directory (optional)
  FbxString lPath = FbxGetApplicationDirectory();
  pManager->LoadPluginsDirectory(lPath.Buffer());

    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "My Scene");
  if( !pScene )
    {
        FBXSDK_printf("Error: Unable to create FBX scene!\n");
        exit(1);
    }
}


void LoadFBX(std::string& fullOutputPath, std::string& namefbxFile, LosBestMesh *lsMesh)
{
  printf(" load Fbx \n "); 

    printf(" name mesh == %s  \n", namefbxFile.c_str());
  
   //nameExampleN_files = losOutputMesh;

 //  nameLosMesh += ".LosModel";

    fullOutputPath +=   namefbxFile;  //nameLosMesh;
 
printf(" all Path to my mesh == %s  \n", fullOutputPath.c_str());

// nameExampleN_files += "ser.dat";

 // FILE* file = fopen(file_name.c_str(), "wb");
 //    if (file == nullptr) {
 //      log_.Log(kLogError, "Could not open %s for writing\n", file_name.c_str());
 //      return;
 //    }

    // Write the binary data to the file and close it.
    // TODO: Add option to write json file too.
    // log_.Log(kLogVerbose, "Writing %s\n", file_name.c_str());
    // fwrite(fbb.GetBufferPointer(), 1, fbb.GetSize(), file);
    // fclose(file);
     
   FbxManager* manager_;// = FbxManager::Create();
   
   FbxScene* scene_ ; //= FbxScene::Create(manager_, "Los Scene"); 
   FbxImporter* importer;
    InitializeSdkObjects(manager_, scene_);

   if (manager_)
   {
       // Create the importer.
       int lFileFormat = -1;
       importer = FbxImporter::Create(manager_,"");
       if (!manager_->GetIOPluginRegistry()->DetectReaderFileFormat(fullOutputPath.c_str(), lFileFormat) )
       {
        printf("WEEE \n\n");
           // Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
          // lFileFormat = manager_->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
        }
       // Initialize the importer by providing a filename.
       if(importer->Initialize(fullOutputPath.c_str(), lFileFormat) == true)
       {
        printf("Import is ok \n");
       }
    }




  FbxArray<FbxString*> mAnimStackNameArray;

   // FbxIOSettings * ios = FbxIOSettings::Create(manager_, IOSROOT);
   //  manager_->SetIOSettings(ios);
     //= FbxImporter::Create(manager_, "");
 //   const bool initial_success = importer->Initialize(fullOutputPath.c_str(), -1, manager_->GetIOSettings());
 //   const FbxStatus init_status = importer->GetStatus();


    if (importer->Import(scene_) == true)
    {
      printf("ok converter \n");
    }else 
    {
      printf("fuck import \n");
    }
  
  //   Initialize     //   was    losNameMesh   
  
    int sdk_major = 0, sdk_minor = 0, sdk_revision = 0;
    int file_major = 0, file_monor = 0, file_revision = 0;

    FbxManager::GetFileFormatVersion(sdk_major, sdk_minor, sdk_revision);
    importer->GetFileVersion(file_major, file_monor, file_revision);

 
    printf(" File version == %d. %d. %d, SDK version == %d. %d. %d \n",
      file_major, file_monor, file_revision, sdk_major, sdk_minor, sdk_revision );


   // const bool import_suc = importer->Import(scene_);
 //   const FbxStatus import_status = importer->GetStatus();

   FbxAxisSystem SceneAxisSystem = scene_->GetGlobalSettings().GetAxisSystem();
            FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
            if( SceneAxisSystem != OurAxisSystem )
            {
                OurAxisSystem.ConvertScene(scene_);
            }

            // Convert Unit System to what is used in this example, if needed
            FbxSystemUnit SceneSystemUnit = scene_->GetGlobalSettings().GetSystemUnit();
            if( SceneSystemUnit.GetScaleFactor() != 1.0 )
            {
                //The unit in this example is centimeter.
                FbxSystemUnit::cm.ConvertScene( scene_);
            }


    

    scene_->FillAnimStackNameArray(mAnimStackNameArray);
 
   const int lAnimStackCoiunt = mAnimStackNameArray.GetCount();

    printf( " lAnimStack == %d  \n", lAnimStackCoiunt);
  //  printf(" my name anim == %s  \n", mAnimStackNameArray);


// mScene->FillAnimStackNameArray(mAnimStackNameArray);
 
  
    FbxGeometryConverter lGeomConvert(manager_);
   // lGeomConvert.RecenterSceneToWorldCenter(scene_, 0.0);
    printf("this read fbx los \n");
    //lGeomConvert.SplitMeshesPerMaterial(scene_, true);   // ? просто списал оттуда, чёрт знает что значит 
     printf("this read fbx los2\n");
     lGeomConvert.Triangulate(scene_, true);



 importer->Destroy();


 printf("this read fbx los 3\n");
     FbxNode *nodeRoot = scene_->GetRootNode();

     printf("my rootNode number == %s \n", nodeRoot->GetName());

     LoadCacheRecursive(scene_, lsMesh);  // was ls_Mesh
 


  if(AnimTrue)
  {
 

     FbxNode* const root_node = scene_->GetRootNode();
    const int child_count = root_node->GetChildCount();
    NodeToBoneMap node_to_bone_map;
    printf("child_count == %d  \n", child_count);

    // First pass: determine which nodes are to be treated as bones.
    // We skip the root node so it's not included in the bone hierarchy.
    for (int child_index = 0; child_index != child_count; ++child_index) {
      FbxNode* const child_node = root_node->GetChild(child_index);
     // MarkBoneNodesRecursive(&node_to_bone_map, child_node); 
      LosOne(&node_to_bone_map, child_node);
      }



    // Second pass: add bones.
    // We skip the root node so it's not included in the bone hierarchy.
    for (int child_index = 0; child_index != child_count; ++child_index) {
      FbxNode* const child_node = root_node->GetChild(child_index);
     // GatherBonesRecursive(&node_to_bone_map, child_node, -1, out);
      LosBoneRecursive(&node_to_bone_map, child_node, -1);
    }


  // Final pass: Traverse the scene and output one surface per mesh.
   // GatherFlatMeshRecursive(&node_to_bone_map, root_node, root_node, out);
 LosMesGatherRecursive(&node_to_bone_map, root_node, root_node, scene_);
 //void LosMesGatherRecursive(const NodeToBoneMap* node_to_bone_map, FbxNode* node, FbxNode* parent_node);

  } // final AnimTrue;

} // Final TODO!  

 // MarkBoneNodesRecursive

  bool LosOne(NodeToBoneMap* node_to_bone_map,
                              FbxNode* node) {
    // We need a bone for this node if it has a skeleton attribute or a mesh.
    bool need_bone = (node->GetSkeleton() || node->GetMesh());

    // We also need a bone for this node if it has any such child bones.
    const int child_count = node->GetChildCount();
    for (int child_index = 0; child_index != child_count; ++child_index) {
      FbxNode* const child_node = node->GetChild(child_index);
      if (LosOne(node_to_bone_map, child_node)) {
        need_bone = true;
      }
    }

    // Flag the node as a bone.
    if (need_bone) {
      printf(" node to Bone !; \n");
      node_to_bone_map->insert(NodeToBoneMap::value_type(node, -1));
    }
    return need_bone;
  }



 //  GatherBonesRecursive
  void LosBoneRecursive(NodeToBoneMap* node_to_bone_map, FbxNode* node,
                            int parent_bone_index) {
    const int bone_index = AddBoneForNode(node_to_bone_map, node, parent_bone_index); // was in final out !
    printf(" Bone index  in LosBoneRecursive == %d \n", bone_index );
    if (bone_index >= 0) {
      const int child_count = node->GetChildCount();
      for (int child_index = 0; child_index != child_count; ++child_index) {
        FbxNode* const child_node = node->GetChild(child_index);
        LosBoneRecursive(node_to_bone_map, child_node, bone_index);
      }
    }
  }


  // unsigned int AppendBone(const char* bone_name,
  //                         const mat4& default_bone_transform_inverse,
  //                         int parent_bone_index) {
  //   const unsigned int bone_index = static_cast<unsigned int>(bones_.size());
  //   bones_.push_back(
  //       Bone(bone_name, default_bone_transform_inverse, parent_bone_index));
  //   return bone_index;
  // }



  //         AddBoneForNode    
  static int AddBoneForNode(NodeToBoneMap* node_to_bone_map, FbxNode* node,
                            int parent_bone_index) {
    // The node is a bone if it was marked as one by MarkBoneNodesRecursive.
    const auto found_it = node_to_bone_map->find(node);
    if (found_it == node_to_bone_map->end()) {
      return -1;
    }

    // Add the bone entry.
    const FbxAMatrix global_transform = node->EvaluateGlobalTransform();
    const FbxAMatrix default_bone_transform_inverse =
        global_transform.Inverse();
    const char* const name = node->GetName();
     printf(" bone name == %s \n", name);
   // const unsigned int bone_index = out->AppendBone(
   //     name, Mat4FromFbx(default_bone_transform_inverse), parent_bone_index);
  
  const unsigned int bone_index = 1;
   found_it->second = bone_index;
    return bone_index;
   // return 1;
  }

//     Transforms

  void LosTransforms(FbxNode* node, FbxNode* parent_node,
                  FbxAMatrix* default_bone_transform_inverse,
                  FbxAMatrix* point_transform) {
    // geometric_transform is applied to each point, but is not inherited
    // by children.
    const FbxVector4 geometric_translation =
        node->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 geometric_rotation =
        node->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 geometric_scaling =
        node->GetGeometricScaling(FbxNode::eSourcePivot);
    const FbxAMatrix geometric_transform(geometric_translation,
                                         geometric_rotation, geometric_scaling);

    const FbxAMatrix global_transform = node->EvaluateGlobalTransform();
    const FbxAMatrix parent_global_transform =
        parent_node->EvaluateGlobalTransform();

    // We want the root node to be the identity. Everything in object space
    // is relative to the root.
    *default_bone_transform_inverse = global_transform.Inverse();
    *point_transform = global_transform * geometric_transform;
  }





  //   GatherSkinBindings
  void LosGatherSkinBindings(const FbxMesh* mesh,
                          SkinBinding::BoneIndex transform_bone_index,
                          const NodeToBoneMap* node_to_bone_map,
                          std::vector<SkinBinding>* out_skin_bindings) {
   
    const unsigned int point_count = mesh->GetControlPointsCount();
    std::vector<SkinBinding> skin_bindings(point_count);
printf(" 111 \n");
    // Each cluster stores a mapping from a bone to all the vertices it
    // influences.  This generates an inverse mapping from each point to all
    // the bones influencing it.
    const int skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int skin_index = 0; skin_index != skin_count; ++skin_index) {
      const FbxSkin* const skin = static_cast<const FbxSkin*>(
          mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
      const int cluster_count = skin->GetClusterCount();
      for (int cluster_index = 0; cluster_index != cluster_count;
           ++cluster_index) {
        const FbxCluster* const cluster = skin->GetCluster(cluster_index);
        const FbxNode* const link_node = cluster->GetLink();
printf(" 222 \n");
        // Get the bone index from the node pointer.
        const NodeToBoneMap::const_iterator link_it =
            node_to_bone_map->find(link_node);
        assert(link_it != node_to_bone_map->end());
        const int bone_index = link_it->second;
printf(" 333 \n");
        // We currently only support normalized weights.  Both eNormalize and
        // eTotalOne can be treated as normalized, because we renormalize
        // weights after extraction.
        const FbxCluster::ELinkMode link_mode = cluster->GetLinkMode();
        if (link_mode != FbxCluster::eNormalize &&
            link_mode != FbxCluster::eTotalOne) {
          // log_.Log(kLogWarning,
          //          "Mesh %s skin %d(%s) cluster %d(%s) has"
          //          " unsupported LinkMode %d (only eNormalize(%d) and"
          //          " eTotalOne(%d) are supported).\n",
          //          GetMeshOrNodeName(mesh), skin_index, skin->GetName(),
          //          cluster_index, cluster->GetName(),
          //          static_cast<int>(link_mode),
          //          static_cast<int>(FbxCluster::eNormalize),
          //          static_cast<int>(FbxCluster::eTotalOne));
          printf(" Normalization problems ! \n");
        }

printf(" 444 \n");
        // Assign bone weights to all cluster influences.
        const int influence_count = cluster->GetControlPointIndicesCount();
        const int* const point_indices = cluster->GetControlPointIndices();
        const double* const weights = cluster->GetControlPointWeights();
        for (int influence_index = 0; influence_index != influence_count;
             ++influence_index) {
          const int point_index = point_indices[influence_index];
          assert(static_cast<unsigned int>(point_index) < point_count);
          const float weight = static_cast<float>(weights[influence_index]);
          printf(" 445 \n");
          skin_bindings[point_index].AppendInfluence(bone_index, weight,
                                                     mesh, point_index);
        }
      }
    }

  printf(" 555 \n");
    // Normalize weights.
    for (unsigned int point_index = 0; point_index != point_count;
         ++point_index) {
      SkinBinding* const skin_binding = &skin_bindings[point_index];
      if (!skin_binding->HasInfluences()) {
        // Any non-skinned vertices not bound to a deformer are implicitly bound
        // to their parent transform.
        skin_binding->BindRigid(transform_bone_index);
      } else {
        skin_binding->NormalizeBoneWeights();
      }
    }

    out_skin_bindings->swap(skin_bindings);
  }





 const FbxGeometryElementUV* UvElements(
      const FbxMesh* mesh, const FbxGeometryElementUV** uv_alt_element) {
    const int uv_count = mesh->GetElementUVCount();
  
//const FbxGeometryElementUV* uv_alt_element = nullptr;
    const FbxGeometryElementUV* uv_element = nullptr;
  //  *uv_alt_element = nullptr;

    // Use the first UV set as the primary UV set.
    if (uv_count > 0) {
      uv_element = mesh->GetElementUV(0);
      printf("Using UV map %s for mesh %s.\n",
               uv_element->GetName(), mesh->GetName());
    }

    // Use the second UV set if it exists.
    if (uv_count > 1) {
      ///*uv_alt_element = mesh->GetElementUV(1);
     // printf("Using alternate UV map %s for mesh %s.\n",
            //   (*uv_alt_element)->GetName(), mesh->GetName());
    }

    // Warn when more UV sets exist.
    if (uv_count > 2) {
      FbxStringList uv_set_names;
      mesh->GetUVSetNames(uv_set_names);
      //printf( "Multiple UVs for mesh %s. Using %s and %s. Ignoring %s.\n",
           //    mesh->GetName(), uv_set_names.GetStringAt(0),
             //  uv_set_names.GetStringAt(1), uv_set_names.GetStringAt(2));
    }


    return uv_element;
  }







//  GatherFlatSurface
  void LosMainGatherFSurface(const FbxMesh* mesh,
                         SkinBinding::BoneIndex transform_bone_index,
                         const NodeToBoneMap* node_to_bone_map,
                         const FbxAMatrix& point_transform)
  {

  
    printf(" LosMainGatherFSurface \n");

  const FbxAMatrix& t = point_transform;
    printf( "    transform: {%.3f %.3f %.3f %.3f}\n"
             "               {%.3f %.3f %.3f %.3f}\n"
             "               {%.3f %.3f %.3f %.3f}\n"
             "               {%.3f %.3f %.3f %.3f}\n",
             t[0][0], t[0][1], t[0][2], t[0][3], t[1][0], t[1][1], t[1][2],
             t[1][3], t[2][0], t[2][1], t[2][2], t[2][3], t[3][0], t[3][1],
             t[3][2], t[3][3]);

    // Affine matrix only supports multiplication by a point, not a vector.
    // That is, there is no way to ignore the translation (as is required
    // for normals and tangents). So, we create a copy of `transform` that
    // has no translation.
    // http://forums.autodesk.com/t5/fbx-sdk/matrix-vector-multiplication/td-p/4245079
    FbxAMatrix vector_transform = point_transform;
    vector_transform.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));

    std::vector<SkinBinding> skin_bindings;
  //  GatherSkinBindings(mesh, transform_bone_index, node_to_bone_map,
     //                  &skin_bindings);
LosGatherSkinBindings(mesh, transform_bone_index, node_to_bone_map, &skin_bindings);




 // Get references to various vertex elements.
    const FbxVector4* vertices = mesh->GetControlPoints();
    const FbxGeometryElementNormal* normal_element = mesh->GetElementNormal();
    const FbxGeometryElementTangent* tangent_element = mesh->GetElementTangent();
    const FbxGeometryElementVertexColor* color_element = mesh->GetElementVertexColor();
    const FbxGeometryElementUV* uv_alt_element = nullptr;
    const FbxGeometryElementUV* uv_element = UvElements(mesh, &uv_alt_element);

    // Record which vertex attributes exist for this surface.
    // We reported the bone name and parents in AppendBone().
   // const VertexAttributeBitmask surface_vertex_attributes =
    //     kVertexAttributeBit_Bone |
    //     (vertices ? kVertexAttributeBit_Position : 0) |
    //     (normal_element ? kVertexAttributeBit_Normal : 0) |
    //     (tangent_element ? kVertexAttributeBit_Tangent : 0) |
    //     (color_element != nullptr || has_solid_color ? kVertexAttributeBit_Color
    //                                                  : 0) |
    //     (uv_element ? kVertexAttributeBit_Uv : 0) |
    //     (uv_alt_element ? kVertexAttributeBit_UvAlt : 0);
    // out->ReportSurfaceVertexAttributes(surface_vertex_attributes);
    // printf("  color in %s  %s\n",  color_element != nullptr
    //                           ? "Mesh has vertex colors\n"
    //                           : has_solid_color
    //                                 ? "Mesh material has a solid color\n"
    //                                 : "Mesh does not have vertex colors\n");

    // Loop through every poly in the mesh.
    int vertex_counter = 0;
    const int num_polys = mesh->GetPolygonCount();
     printf(" polygonCount == %d \n", num_polys );
    for (int poly_index = 0; poly_index < num_polys; ++poly_index) {
      // Ensure polygon is a triangle. This should be true since we call
      // Triangulate() when we load the scene.
      const int num_verts = mesh->GetPolygonSize(poly_index);
      printf(" numVertex = %d \n", num_verts);
      if (num_verts != 3) {
        printf("mesh %s poly %d has %d verts instead of 3\n",
                 mesh->GetName(), poly_index, num_verts);
        continue;
      }



    // Loop through all three verts.
      for (int vert_index = 0; vert_index < num_verts; ++vert_index) {
        // Get the control index for this poly, vert combination.
        const int control_index = mesh->GetPolygonVertex(poly_index, vert_index);

        // Depending on the FBX format, normals and UVs are indexed either
        // by control point or by polygon-vertex.
        const FbxVector4 vertex_fbx = vertices[control_index];
        const FbxVector4 normal_fbx = ElementFromIndices(normal_element, control_index, vertex_counter);
        const FbxVector4 tangent_fbx = ElementFromIndices(tangent_element, control_index, vertex_counter);
      
        // const FbxColor color_fbx =
        //     color_element != nullptr
        //         ? ElementFromIndices(color_element, control_index,
        //                              vertex_counter)
        //         : has_solid_color ? solid_color : kDefaultColor;
           const FbxVector2 uv_fbx = ElementFromIndices(uv_element, control_index, vertex_counter);
           const FbxVector2 uv_alt_fbx = ElementFromIndices(uv_alt_element, control_index, vertex_counter);

        // // Output this poly-vert.
        // // Note that the v-axis is flipped between FBX UVs and FlatBuffer UVs.
         
          printf(" get and print Data! \n");

 //    was const vec3 const vec3 const vec4
      //   const vec3 vertex = Vec3FromFbx(point_transform.MultT(vertex_fbx));
       //  const vec3 normal = Vec3FromFbx(vector_transform.MultT(normal_fbx)).Normalized();
      //   const vec4 tangent( Vec3FromFbx(vector_transform.MultT(tangent_fbx)).Normalized(), static_cast<float>(tangent_fbx[3]));




           Vec3FromFbx(point_transform.MultT(vertex_fbx));
           //Vec3FromFbx(vector_transform.MultT(normal_fbx)).Normalized();
           Vec3FromFbx(vector_transform.MultT(normal_fbx));
           //Vec3FromFbx(vector_transform.MultT(tangent_fbx)).Normalized(), static_cast<float>(tangent_fbx[3]));
           Vec3FromFbx(vector_transform.MultT(tangent_fbx));

        // const vec4 color = Vec4FromFbx(color_fbx);

        // const vec2 uv = Vec2FromFbxUv(uv_fbx);
        // const vec2 uv_alt = Vec2FromFbxUv(uv_alt_fbx);
           Vec2FromFbxUv(uv_fbx);
           Vec2FromFbxUv(uv_alt_fbx);

           const SkinBinding& skin_binding = skin_bindings[control_index];
        // out->AppendPolyVert(vertex, normal, tangent, color, uv, uv_alt,
        //                     skin_binding);

        // Control points are listed in order of poly + vertex.
        vertex_counter++;
      }

 
  }




  }





   // GatherFlatMeshRecursive   ****************  ...FlatMesh* out)

  // For each mesh in the tree of nodes under `node`, add a surface to `out`.
  void LosMesGatherRecursive(const NodeToBoneMap* node_to_bone_map,
                               FbxNode* node, FbxNode* parent_node, FbxScene* scene_){
    // We're only interested in mesh nodes. If a node and all nodes under it
    // have no meshes, we early out.
    if (node == nullptr || !fplutil::NodeHasMesh(node)) return;
     printf("  Node: %s \n", node->GetName());

    // The root node cannot have a transform applied to it, so we do not
    // export it as a bone.
    int bone_index = -1;
    if (node != scene_->GetRootNode()) {
      // Get the transform to this node from its parent.
      FbxAMatrix default_bone_transform_inverse;
      FbxAMatrix point_transform;
     

      //Transforms(node, parent_node, &default_bone_transform_inverse,
          //       &point_transform);
      LosTransforms(node, parent_node, &default_bone_transform_inverse,
                     &point_transform);

      // Find the bone for this node.  It must have one, because we checked that
      // it contained a mesh above.
      const auto found_it = node_to_bone_map->find(node);
      assert(found_it != node_to_bone_map->end());
      bone_index = found_it->second;

      // Gather mesh data for this bone.
      // Note that there may be more than one mesh attached to a node.
      for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
        const FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
        if (attr == nullptr ||
            attr->GetAttributeType() != FbxNodeAttribute::eMesh)
          continue;
        const FbxMesh* mesh = static_cast<const FbxMesh*>(attr);

        // Gather the textures attached to this mesh.
        std::string normal_map_file_name;

        printf(" in this to Gather Skin  \n");
       // const FlatTextures textures = GatherTextures(node, mesh);
       // out->SetSurface(textures);

        // If no textures for this mesh, try to get a solid color from the
        // material.
        FbxColor solid_color;
       // const bool has_solid_color =
       //     textures.Count() == 0 && SolidColor(node, mesh, &solid_color);

        // Without a base texture or color, the model will look rather plane.
      //  if (textures.Count() == 0 && !has_solid_color) {
       //   log_.Log(kLogWarning, "No texture or solid color found for node %s\n",
       //            node->GetName());
      //  }

        // Gather the verticies and indices.
      //  GatherFlatSurface(
      //      mesh, static_cast<SkinBinding::BoneIndex>(bone_index),
       //     node_to_bone_map, point_transform, has_solid_color, solid_color,
         //   out);
 

  LosMainGatherFSurface(mesh, static_cast<SkinBinding::BoneIndex>(bone_index), node_to_bone_map, point_transform);




      }
    }

    // Recursively traverse each node in the scene
    for (int i = 0; i < node->GetChildCount(); i++) {
     // GatherFlatMeshRecursive(node_to_bone_map, node->GetChild(i), node, out);
      LosMesGatherRecursive(node_to_bone_map, node->GetChild(i), node, scene_);
    }
  }
















typedef unsigned char  uint8;
typedef   signed char   int8;
typedef unsigned short uint16;
typedef   signed short  int16;
typedef unsigned int   uint32;
typedef   signed int    int32;

 inline uint16 FloatToHalf( float v )
{
    uint32 src = *(uint32*)(&v);
    uint32 sign = src >> 31;   // Shifts on unsigned int types are always supposed to zero fill, is this the case for Mac, and Linux, etc. as well..
    uint16 ret;

    // Extract mantissa
    uint32 mantissa = src  &  (1 << 23) - 1;

    // extract & convert exp bits
    int32 exp = int32( src >> 23  &  0xFF ) - 127;
    if( exp > 15 )
    {
        // The largest-possible positive 2-byte-float exponent value would be 11110-01111 = 11110 = 15.
        exp = 15;
        mantissa = (1 << 23) - 1;
    }
    else if( exp < -14 )
    {
        // handle wraparound of shifts (done mod 64 on PPC)
        // All float32 denormal/0 values map to float16 0
        if( exp <= - 14 - 24 )
        {
            mantissa = 0;
        }
        else
        {
            mantissa = ( mantissa | 1 << 23 )  >>  (-exp - 14);
        }
        exp = -14;
    }
    // TODO: exp is a *signed* int, left shifting it could extend the signed bit,
    // will have to mask off the first bits where the mantissa should go.
    ret = (sign << 15)  |  ((uint16)(exp + 15)  << 10) |  (mantissa >> 13);
    return ret;
}





bool InitializeFbxMesh(const FbxMesh *pMesh, LosBestMesh *meshL)
{
 
  printf(" in initialzizeFbxMesh \n");
    if(!pMesh->GetNode())
     return false;

  bool mAllByControlPoint;

     //printf(" los String == %s \n", meshL.ls_name_mesh.c_str());

 int numberV;
 

      FbxVector4* verticesTwo = pMesh->GetControlPoints();
          
           const int losCountAll= pMesh->GetPolygonCount();
            printf(" my losCountAll %d \n", losCountAll);

           // FbxArray<SubMesh*> mSubMeshes;

    FbxLayerElementArrayTemplate<int>* lMaterialIndice = nullptr;
    FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
    if (pMesh->GetElementMaterial())
    {
        lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
        lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
        if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
        {
            FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
            if (lMaterialIndice->GetCount() == losCountAll)
            {
                // Count the faces of each material
                for (int lPolygonIndex = 0; lPolygonIndex < losCountAll; ++lPolygonIndex)
                {
                    const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
                    if (mSubMeshes.GetCount() < lMaterialIndex + 1)
                    {
                        mSubMeshes.Resize(lMaterialIndex + 1);
                    }
                    if (mSubMeshes[lMaterialIndex] == NULL)
                    {
                        mSubMeshes[lMaterialIndex] = new SubMesh;
                    }
                    mSubMeshes[lMaterialIndex]->TriangleCount += 1;
                }

                // Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
                // if, in the loop above, we resized the mSubMeshes by more than one slot.
                for (int i = 0; i < mSubMeshes.GetCount(); i++)
                {
                    if (mSubMeshes[i] == NULL)
                        mSubMeshes[i] = new SubMesh;
                }

                // Record the offset (how many vertex)
                const int lMaterialCount = mSubMeshes.GetCount();
                int lOffset = 0;
                for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
                {
                    mSubMeshes[lIndex]->IndexOffset = lOffset;
                    lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
                    // This will be used as counter in the following procedures, reset to zero
                    mSubMeshes[lIndex]->TriangleCount = 0;
                }
                FBX_ASSERT(lOffset == lPolygonCount * 3);
            }
        }
    }

    // All faces will use the same material.
    if (mSubMeshes.GetCount() == 0)
    {
        mSubMeshes.Resize(1);
        mSubMeshes[0] = new SubMesh();
    }

    //   std::vector<float> v;
            
    //    for( int j = 0; j < losCountAll; j++)
    //    {
    //     numberV = pMesh->GetPolygonSize(j);
    //      assert(numberV >= 0);
    //       for( int h = 0; h < numberV; h++)
    //       {
    //          int ControlPointIndex = pMesh->GetPolygonVertex(j, h);
    //          printf("%ff, %ff, %ff,  \n", (float)verticesTwo[ControlPointIndex].mData[0], (float)verticesTwo[ControlPointIndex].mData[1],
    //                    (float)verticesTwo[ControlPointIndex].mData[2]);
    //          v.push_back(verticesTwo[ControlPointIndex].mData[0]);
    //          v.push_back(verticesTwo[ControlPointIndex].mData[1]);
    //          v.push_back(verticesTwo[ControlPointIndex].mData[2]);
    //       }
    //    }

    bool mHasNormal;
    bool mHasUV;
    bool hasTangentExists;
    bool hasBiNormalExists;
    mHasNormal = pMesh->GetElementNormalCount() > 0;
    mHasUV = pMesh->GetElementUVCount() > 0;

// PMesh->GetElementTangentCount()
    hasTangentExists = pMesh->GetElementTangentCount() > 0;
    hasBiNormalExists = pMesh->GetElementBinormalCount() > 0;

    if(mHasUV)
        printf("UV ok \n");


    if(hasTangentExists)
      printf(" tangents ok \n");

    if(hasBiNormalExists)
      printf("BiNormal ok  \n");

    printf("Elements Tangents ==%d \n", pMesh->GetElementTangentCount());
     printf("Elements Binormal ==%d \n", pMesh->GetElementBinormalCount());

   FbxGeometryElementTangent* TanElement;
    for(int t = 0; t < pMesh->GetElementTangentCount(); t++)
    {
 //FbxGeometryElementTangent*
     const FbxGeometryElementTangent* TanElement = pMesh->GetElementTangent(t);

       if(TanElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
           printf("Tangent's this ! \n");
      for (int VertIndex = 0; VertIndex < pMesh->GetControlPointsCount(); VertIndex++)
      {
        int TangentIndex = 0;

        if (TanElement->GetReferenceMode() == FbxGeometryElement::eDirect)
        {
        //   printf("TangentIndex == !! \n");
          TangentIndex = VertIndex;
        }

        if (TanElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
        {
          TangentIndex = TanElement->GetIndexArray().GetAt(TangentIndex);
        }
      
        FbxVector4 Tangents = TanElement->GetDirectArray().GetAt(TangentIndex);
   
      //  printf("Tangents 0 == %f \n", Tangents.mData[0]);
       // printf("Tangents 1 == %f \n", Tangents.mData[1]);
        //printf("Tangents 2 == %f \n", Tangents.mData[2]);

        // OutVertexVector->at(VertIndex).Tan[0] = Tangents.mData[0];
        // OutVertexVector->at(VertIndex).Tan[1] = Tangents.mData[1];
        // OutVertexVector->at(VertIndex).Tan[2] = Tangents.mData[2];
      }
    }

    if (TanElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
     // for (int VertIndex = 0; VertIndex < pMesh->GetControlPointsCount(); VertIndex++)
     // {
      //   int TangentIndex = 0;
             printf("Tangents two this \n");
      //   //If reference mode is direct that means that the Tangent index and the vertex index are the same
      //   if (TanEle->GetReferenceMode() == FbxGeometryElement::eDirect)
      //   {
      //     TangentIndex = VertIndex;
      //   }

      //   //If reference mode is Index to Direct, that means that the Tangents are collected by Index to Direct
      //   if (TanEle->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
      //   {
      //     TangentIndex = TanEle->GetIndexArray().GetAt(TangentIndex);
      //   }

      //   FbxVector4 Tangents = TanEle->GetDirectArray().GetAt(TangentIndex);

      //   OutVertexVector->at(VertIndex).Tan[0] = Tangents.mData[0];
      //   OutVertexVector->at(VertIndex).Tan[1] = Tangents.mData[1];
      //   OutVertexVector->at(VertIndex).Tan[2] = Tangents.mData[2];
      // }
  //  }

    }
}


// animation branch !!! 

 // ***********  BINormal 
   for(int t = 0; t < pMesh->GetElementBinormalCount(); t++)
    {
 //FbxGeometryElementTangent*
     const FbxGeometryElementBinormal* BiNormalEleme = pMesh->GetElementBinormal(t);

       if(BiNormalEleme->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
           printf("Binormal's this ! \n");
      for (int VertIndex = 0; VertIndex < pMesh->GetControlPointsCount(); VertIndex++)
      {
        int BinormalIndex = 0;

        if (BiNormalEleme->GetReferenceMode() == FbxGeometryElement::eDirect)
        {
          BinormalIndex = VertIndex;
        }

        if (BiNormalEleme->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
        {
          BinormalIndex = BiNormalEleme->GetIndexArray().GetAt(BinormalIndex);
        }
      
        FbxVector4 Binorm = BiNormalEleme->GetDirectArray().GetAt(BinormalIndex);
     // printf("vertIndex == %d \n", VertIndex);
        // printf("BiNormal 0 == %f \n", Binorm.mData[0]);
        // printf("BiNormal 1 == %f \n", Binorm.mData[1]);
        // printf("BiNormal 2 == %f \n", Binorm.mData[2]);

// BiNormal 0 == 0.073786 
// BiNormal 1 == 0.985332 
// BiNormal 2 == -0.153875 
        // OutVertexVector->at(VertIndex).Tan[0] = Tangents.mData[0];
        // OutVertexVector->at(VertIndex).Tan[1] = Tangents.mData[1];
        // OutVertexVector->at(VertIndex).Tan[2] = Tangents.mData[2];
      }
    }

    if (BiNormalEleme->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
     // for (int VertIndex = 0; VertIndex < pMesh->GetControlPointsCount(); VertIndex++)
     // {
      //   int TangentIndex = 0;
             printf("Tangents two this \n");
      //   //If reference mode is direct that means that the Tangent index and the vertex index are the same
      //   if (TanEle->GetReferenceMode() == FbxGeometryElement::eDirect)
      //   {
      //     TangentIndex = VertIndex;
      //   }

      //   //If reference mode is Index to Direct, that means that the Tangents are collected by Index to Direct
      //   if (TanEle->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
      //   {
      //     TangentIndex = TanEle->GetIndexArray().GetAt(TangentIndex);
      //   }

      //   FbxVector4 Tangents = TanEle->GetDirectArray().GetAt(TangentIndex);

      //   OutVertexVector->at(VertIndex).Tan[0] = Tangents.mData[0];
      //   OutVertexVector->at(VertIndex).Tan[1] = Tangents.mData[1];
      //   OutVertexVector->at(VertIndex).Tan[2] = Tangents.mData[2];
      // }
  //  }

    }
}








     FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
    FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
    FbxGeometryElement::EMappingMode lTangentsMode = FbxGeometryElement::eNone;
      FbxGeometryElement::EMappingMode lBinormalMode = FbxGeometryElement::eNone;

  if (mHasNormal)
    {
        lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
        if (lNormalMappingMode == FbxGeometryElement::eNone)
        {
            mHasNormal = false;
        }
        if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
        {
            mAllByControlPoint = false;
        }
    }
    if (mHasUV)
    {
        lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
        if (lUVMappingMode == FbxGeometryElement::eNone)
        {
            mHasUV = false;
        }
        if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
        {
            mAllByControlPoint = false;
        }
    }

        if (hasTangentExists)
    {
        lTangentsMode = pMesh->GetElementTangent(0)->GetMappingMode();
        if (lTangentsMode == FbxGeometryElement::eNone)
        {
            hasTangentExists = false;
        }
        if (hasTangentExists && lTangentsMode != FbxGeometryElement::eByControlPoint)
        {
            mAllByControlPoint = false;
        }
    }

    // hasBiNormalExists
        if (hasBiNormalExists)
      {
        lBinormalMode = pMesh->GetElementTangent(0)->GetMappingMode();
        if (lBinormalMode == FbxGeometryElement::eNone)
        {
            hasBiNormalExists = false;
        }
        if (hasTangentExists && lBinormalMode != FbxGeometryElement::eByControlPoint)
        {
            mAllByControlPoint = false;
        }
      }




int lPolygonVertexCount = pMesh->GetControlPointsCount();
    if (!mAllByControlPoint)
    {               // was lPolygonCount losCountAll
        lPolygonVertexCount = losCountAll * TRIANGLE_VERTEX_COUNT;
    }
    auto * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
    printf("lPolygonVertexCount * VERTEX_STRIDE == %d \n", lPolygonVertexCount * VERTEX_STRIDE);
    printf("lPolygonVertexCount * NORMAL_STRIDE == %d \n", lPolygonVertexCount * NORMAL_STRIDE);
    auto * lLosVertex = new float[lPolygonVertexCount * NORMAL_STRIDE];
    auto * lIndices = new unsigned short[losCountAll * TRIANGLE_VERTEX_COUNT]; // was int
    float * lNormals = nullptr;
    //float * lTangents = nullptr;
    if (mHasNormal)
    {
        lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
    }
    float * lUVs = nullptr;
    FbxStringList lUVNames;
    pMesh->GetUVSetNames(lUVNames);
    const char * lUVName = nullptr;
    if (mHasUV && lUVNames.GetCount())
    {
        lUVs = new float[lPolygonVertexCount * UV_STRIDE];
        lUVName = lUVNames[0];
    }

    // Populate the array with vertex attribute, if by control point.
    const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
    FbxVector4 lCurrentVertex;
    FbxVector4 lCurrentNormal;
    FbxVector2 lCurrentUV;

 printf(" in too 111333 II888  \n");
 if (mAllByControlPoint)
    {
        const FbxGeometryElementNormal * lNormalElement = nullptr;
        const FbxGeometryElementUV * lUVElement = nullptr;
        if (mHasNormal)
        {
            lNormalElement = pMesh->GetElementNormal(0);
        }
        if (mHasUV)
        {
            lUVElement = pMesh->GetElementUV(0);
        }
        for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
        {
            // Save the vertex position.
            lCurrentVertex = lControlPoints[lIndex];
            lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
            lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
            lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
            lVertices[lIndex * VERTEX_STRIDE + 3] = 1;


            lLosVertex[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
            lLosVertex[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
            lLosVertex[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
            // Save the normal.
            if (mHasNormal)
            {
                int lNormalIndex = lIndex;
                if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                {
                    lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
                }
                lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
                lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
                lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
                lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
            }

            // Save the UV.
            if (mHasUV)
            {
                int lUVIndex = lIndex;
                if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                {
                    lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
                }
                lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
                lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
                lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
                printf("lUVs 1 and 2 == %f, %f \n", static_cast<float>(lCurrentUV[0]), static_cast<float>(lCurrentUV[1]));
            }
        }

    }
  int lVertexCount = 0;              // lPolygonCount 
 const FbxGeometryElementTangent* tanGoIndexLos = pMesh->GetElementTangent(0);
    for (int lPolygonIndex = 0; lPolygonIndex < losCountAll; ++lPolygonIndex)
    {
        // The material for current face.
        int lMaterialIndex = 0;
        if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
        {
            lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
        }

  

        // Where should I save the vertex attribute index, according to the material
       const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
           mSubMeshes[lMaterialIndex]->TriangleCount * 3; // was TRIANGLE_VERTEX_COUNT
        for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
        {
            const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

            if (mAllByControlPoint)
            {
              printf("mAllByControl \n");
                lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned short>(lControlPointIndex);
                printf("mAllByControl  == %d \n", lIndices[lIndexOffset + lVerticeIndex]);
            }
            // Populate the array with vertex attribute, if by polygon vertex.
            else
            { 
                   const auto indexLosToFor = lIndexOffset + lVerticeIndex; // change unsigned int to uint32_t
                lIndices[indexLosToFor] = static_cast<uint32_t>(lVertexCount);
            //printf("index %d  == %d \n", indexLosToFor, lIndices[indexLosToFor]);

                lCurrentVertex = lControlPoints[lControlPointIndex];
                lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
                lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
                lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
                lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;


                if (mHasNormal)
                {
                  
                    pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
                   
                    lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
                    lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
                    lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
                 //   printf("Normals == %f, %f \n", static_cast<float>(lCurrentNormal[0]), static_cast<float>(lCurrentNormal[1]) );
                }
                if (mHasUV)
                {
                  
                 
                   bool lUnmappedUV;
                   pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
            
                    lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
                    lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

      printf("lUVs == 1 and 2 %f, %f \n", static_cast<float>(lCurrentUV[0]), static_cast<float>(lCurrentUV[1]) );
                }

                if(hasTangentExists)
                {  // tanGoIndexLos  lVerticeIndex
                  // TangentIndex = TanEle->GetIndexArray().GetAt(TangentIndex);  // lPolygonIndex
                  int tanI = 0;
                  tanI = tanGoIndexLos->GetIndexArray().GetAt(1);
                 //const int con = tanGoIndexLos->GetIndexCount();
                  //printf("tanI == %d, lPolygonIndex = %d  \n", tanI, lPolygonIndex);
                  FbxVector4 Tangents = tanGoIndexLos->GetDirectArray().GetAt(tanI);

                 // printf("LosTangents 0 == %f \n", Tangents.mData[0]);
                 // printf("LosTangents 1 == %f \n", Tangents.mData[1]);
                 // printf("LosTangents 2 == %f \n", Tangents.mData[2]);
                // OutVertexVector->at(VertIndex).Tan[1] = Tangents.mData[1];
                // OutVertexVector->at(VertIndex).Tan[2] = Tangents.mData[2];
                }
          
            }
            ++lVertexCount;
        }
   
        mSubMeshes[lMaterialIndex]->TriangleCount += 1;
    }
  printf(" allk OKKK  \n");

    
 
     const auto couT = losCountAll* VERTEX_STRIDE; // = 75518] 
    auto  vVCount = 0;
     // lPolygonVertexCount * VERTEX_STRIDE  == lVertices ==  56640
    const auto aCouintEr = (lPolygonVertexCount * VERTEX_STRIDE)/4;
     for(auto aIter = 0; aIter < aCouintEr; ++aIter)
     {
         printf("lVertices[%d] == %f \n", vVCount * VERTEX_STRIDE, lVertices[vVCount * VERTEX_STRIDE]);
         printf("lVertices[%d] == %f \n", vVCount * VERTEX_STRIDE + 1, lVertices[vVCount * VERTEX_STRIDE + 1]);
          printf("lVertices[%d] == %f \n", vVCount * VERTEX_STRIDE + 2, lVertices[vVCount * VERTEX_STRIDE + 2]);
         lLosVertex[vVCount * NORMAL_STRIDE] = lVertices[vVCount * VERTEX_STRIDE];
         lLosVertex[vVCount * NORMAL_STRIDE + 1] = lVertices[vVCount * VERTEX_STRIDE + 1];
         lLosVertex[vVCount * NORMAL_STRIDE + 2] = lVertices[vVCount * VERTEX_STRIDE + 2];
         // lVertices[vVCount * VERTEX_STRIDE + 1] ;
         // lVertices[vVCount * VERTEX_STRIDE + 2] ;
         ++vVCount;
     }

    // [](double d) { return d < 0.00001 ? 0 : d; }
printf(" we are thw !3232 \n");
      //using losHort = unsigned short; 
    auto UNumber = aCouintEr * 3;
    vVCount = 0;
    auto * halfFloatdata = new uint16[UNumber];
      for(auto inter = 0; inter < aCouintEr; ++inter)
      {
   halfFloatdata[vVCount * NORMAL_STRIDE]   = FloatToHalf(lLosVertex[vVCount * NORMAL_STRIDE]);
   halfFloatdata[vVCount * NORMAL_STRIDE+1] = FloatToHalf(lLosVertex[vVCount * NORMAL_STRIDE+1]);
   halfFloatdata[vVCount * NORMAL_STRIDE+2] = FloatToHalf(lLosVertex[vVCount * NORMAL_STRIDE+2]);

    ++vVCount; 
      }
printf(" we are thw !3232  566\n");
      // ************************************************************************************
      // **********  UV FloatToHalf *********************************************************

//mHasUV  mHasNormal

    auto UNumber2 = aCouintEr * 2;
        vVCount = 0;
         auto* halfFloatdataUV = new uint16[UNumber2];

      if(mHasUV) {
 
printf(" we are pre go UV \n");
      // lPolygonVertexCount * UV_STRIDE
    for(auto inter = 0; inter < aCouintEr; ++inter)
      {
        printf(" in UNV for loop intrio vVCount ==  %d \n", vVCount);
        printf(" in UNV for loop intrio vVCount * UV_STRIDE==  %d \n", vVCount * UV_STRIDE);
        printf(" lUVs[] == %f \n", lUVs[0]);

       printf("lUVs[%d] == %f \n", vVCount * UV_STRIDE, lUVs[vVCount * UV_STRIDE] );
       printf("lUVs[%d] == %f \n", vVCount * UV_STRIDE + 1, lUVs[vVCount * UV_STRIDE + 1]);
       //   printf("lLosVertex[%d] == %f \n", vVCount * NORMAL_STRIDE + 2, lLosVertex[vVCount * NORMAL_STRIDE + 2]);
   halfFloatdataUV[vVCount * UV_STRIDE]   = FloatToHalf(lUVs[vVCount * UV_STRIDE]);
   halfFloatdataUV[vVCount * UV_STRIDE+1] = FloatToHalf(lUVs[vVCount * UV_STRIDE+1]);

    ++vVCount; 
      }
} // end if (mHasUV)

printf(" we are thw !3232  5986897666\n");
   vVCount = 0;
   auto* halfToNormal = new uint16[UNumber];
    if(mHasNormal ){ 
   //uint16* halfToNormal = new uint16[UNumber];
  // lNormals
       for(auto lNormInte = 0; lNormInte < aCouintEr; ++lNormInte)
       {
        printf(" losNormals ==      %f \n", vVCount * NORMAL_STRIDE, lNormals[vVCount * NORMAL_STRIDE]);
        printf(" losNormals == %d = %f \n", vVCount * NORMAL_STRIDE + 1, lNormals[vVCount * NORMAL_STRIDE + 1]);
        printf(" losNormals == %d = %f \n", vVCount * NORMAL_STRIDE + 2, lNormals[vVCount * NORMAL_STRIDE + 2]);
       halfToNormal[vVCount * NORMAL_STRIDE] = FloatToHalf(lNormals[vVCount * NORMAL_STRIDE]);
       halfToNormal[vVCount * NORMAL_STRIDE+1] = FloatToHalf(lNormals[vVCount * NORMAL_STRIDE+1]);
       halfToNormal[vVCount * NORMAL_STRIDE+2] = FloatToHalf(lNormals[vVCount * NORMAL_STRIDE+2]);


        ++vVCount;
       }
} // end if(mHasNormal)




printf(" we are thw !3232  099\n");
 //   GLsizei lOffset = mSubMeshes[pMaterialIndex]->IndexOffset * sizeof(unsigned int);
        // auto  vVCount = 0;
        // const auto couT = losCountAll* VERTEX_STRIDE;
        //    printf("tuam pls 00  = %d \n", couT);  // tuam pls 00 =  18880 
        // for(auto kii9 = 0; kii9 < couT; ++kii9)
        // {

        //         lLosVertex[vVCount * NORMAL_STRIDE] = lVertices[vVCount * VERTEX_STRIDE] ;
        //         lLosVertex[vVCount * NORMAL_STRIDE + 1] = lVertices[vVCount * VERTEX_STRIDE + 1] ;
        //         lLosVertex[vVCount * NORMAL_STRIDE + 2] = lVertices[vVCount * VERTEX_STRIDE + 2] ;
        //            printf(" value this ! == %d \n", kii9);
        //            printf(" lLosVertex [%d] == %f \n", vVCount * NORMAL_STRIDE, lLosVertex[vVCount * NORMAL_STRIDE]);
        //            printf(" lLosVertex [%d] == %f \n", vVCount * NORMAL_STRIDE + 1, lLosVertex[vVCount * NORMAL_STRIDE + 1]);
        //            printf(" lLosVertex [%d] == %f \n", vVCount * NORMAL_STRIDE + 2, lLosVertex[vVCount * NORMAL_STRIDE + 2]);
        //   ++vVCount;
        // }
       
// value this ! == 14335 
// lLosVertex [43005] == -0.000000 
// lLosVertex [43006] == -1.796373 
// lLosVertex [43007] == 2.000000 
        // lLosVertex count == 42480 


                printf(" allk OKKK 2222 98   \n");
           // for(int t = 0; t < 100; ++t)
           // {
           //  printf("los lLosVertex[%d], == %f \n", t, lLosVertex[t]);
           //  // printf("los lLosVertex[%d], == %f \n", t, lLosVertex[t]);
           //  // printf("los normal[%d], == %f \n", t, lNormals[t]);
           // }   





    // vertex count == lPolygonVertexCount * VERTEX_STRIDE
    // index count ==  losCountAll * TRIANGLE_VERTEX_COUNT



// float * lLosVertex = new float[lPolygonVertexCount * NORMAL_STRIDE];
       printf("mSubMesh Count == %d \n", mSubMeshes.GetCount() );
       printf("mSubMesh Index  == %d \n", mSubMeshes[0]->TriangleCount *  3);
       printf("mSubMesh offset   == %d \n", mSubMeshes[0]->IndexOffset *  sizeof(unsigned int));

     const auto losIntVertex = lPolygonVertexCount * NORMAL_STRIDE; // was VERTEX_STRIDE  NORMAL_STRIDE
     const auto losINtIndex = losCountAll * TRIANGLE_VERTEX_COUNT;
     const auto losUVsize  = lPolygonVertexCount * UV_STRIDE;
     const auto losN  = lPolygonVertexCount * NORMAL_STRIDE;
       printf("losIntVertex == %d \n", losIntVertex);
       printf("losINtIndex  == %d \n", losINtIndex);

      //     for(int iR = 0; iR < losINtIndex; iR++)
      // {
      //     printf("lIndices[iR] == %d \n", lIndices[iR]);
      // }

       printf("my vertex ! \n"); 
      for(auto ji = 0; ji < losIntVertex; ++ji)
      {
        // losIntVertex
        printf("losIntVertex[%d] == %f \n", ji, lLosVertex[ji]);
      }

      //  for(auto ji = 0; ji < losUVsize; ++ji)
      // {
      //   // losIntVertex
      //   printf("lUVs[%d] == %f \n", ji, lUVs[ji]);
      // }



     std::string filename = "TestArray2.bin";
        std::ofstream losStream(filename, std::ios::binary);

//  vertex Size unsigned int 
       auto losIndeff = static_cast<uint32_t>(losIntVertex);
    losStream.write((const char*)&losIndeff, sizeof(uint32_t));
//  index Size unsigned int 
      auto losIndeff2 = static_cast<uint32_t>(losINtIndex);
    losStream.write((const char*)&losIndeff2, sizeof(uint32_t));

    //  uv Size unsigned int  
    unsigned int losUV  = 0; 
   if(mHasUV){ 
    //was unsigned int losUV 
    losUV = static_cast<uint32_t>(losUVsize);
    losStream.write((const char*)&losUV, sizeof(uint32_t));
  }
    // normals size  
  auto losNormal = static_cast<unsigned int>(losN);
  if(mHasNormal) { 
    
    losStream.write((const char*)&losNormal, sizeof(unsigned int));
  }
 
      for(int i =0; i < losIntVertex; ++i)
      {
        // uint16 * halfFloatdata 
        //  losStream.write((const char*)&lintVect.at(i), sizeof(unsigned int)); // lLosVertex
        //losStream.write((const char*)&lLosVertex[i], sizeof(float));  // was lVertices  lLosVertex
        losStream.write((const char*)&halfFloatdata[i], sizeof(uint16));
      }

       // unsigned int Lokw = vFloatL.size();
 // losStream.write((const char*)&Lokw, sizeof(unsigned int));
        for(int i = 0; i < losINtIndex; i++)
      {
          losStream.write((const char*)&lIndices[i], sizeof(unsigned short));
      }

  // mHasUV  mHasNormal
   if (mHasUV) { 
          for(unsigned int i = 0; i < losUV; i++) // uv write
      {
        // halfFloatdataUV
          // was //losStream.write((const char*)&lUVs[i], sizeof(float));
        losStream.write((const char*)&halfFloatdataUV[i], sizeof(uint16));
      }
    }

      if(mHasNormal) { 

            for(int i = 0; i < losNormal; i++) // uv write  // losN  // losNormal
      {

         // losStream.write((const char*)&lNormals[i], sizeof(unsigned int));
        losStream.write((const char*)&halfToNormal[i], sizeof(uint16));
      }
    }

  losStream.close();
  delete[] halfFloatdata;
  if(mHasUV) { 
  delete[] halfFloatdataUV;
              }
   printf(" pre build binary final \n");



    for(int iR = 0; iR < losINtIndex; iR++)
      {
          printf("lIndices[iR] == %d \n", lIndices[iR]);
      }

    delete[] lUVs;
  //  delete[] lNormals;
    delete[] lVertices;
    delete[] lIndices;
    delete[] lLosVertex;

















//      printf(" eControl UV ! \n\n");
//       FbxStringList UVNameList;
//       pMesh->GetUVSetNames(UVNameList);
//  printf(" eControl UV ! 111 \n\n");
//        for(int SetIndex = 0; SetIndex < UVNameList.GetCount(); SetIndex++)
//        {
//           const char* UVSetName = UVNameList.GetStringAt(SetIndex);
//           const FbxGeometryElementUV* UVEle = pMesh->GetElementUV(UVSetName);

//            if(!UVEle)
//             continue;

//           if(UVEle->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && UVEle->GetMappingMode() != FbxGeometryElement::eByControlPoint)
//             return 1; 

//  printf(" eControl UV 222! \n\n");
//             const bool UseIndex = UVEle->GetReferenceMode() != FbxGeometryElement::eDirect && UVEle->GetReferenceMode() == 
//             FbxGeometryElement::eIndexToDirect;
// printf(" eControl UV !333\n\n");
//            // const int IndexCounter = (UseIndex) ? UVEle->GetIndexArray().GetCount() : 0;
//             printf(" eControl UV !335555\n\n");
//             const int PolyCount = pMesh->GetPolygonCount();

//  printf(" eControl UV !444 \n\n");
//             if(UVEle->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//             {
//               printf(" first  UV \n");
//                for(int polyIndex = 0; polyIndex < PolyCount; polyIndex++)
//                {
//                 const int PolySize = pMesh->GetPolygonSize(polyIndex);
//                  for(int VertexIndex = 0; VertexIndex < PolySize; VertexIndex++)
//                  {
//                      FbxVector2 uv;

//                      int PolyVertexIndex = pMesh->GetPolygonVertex(polyIndex, VertexIndex);
//                      int UVIndex = UseIndex ? UVEle->GetIndexArray().GetAt(UVIndex) : PolyVertexIndex;
//                      uv = UVEle->GetDirectArray().GetAt(UVIndex);

//                  //    printf(" %ff, %ff, \n", uv.mData[0], 1 - uv.mData[1]);
//                  }
//                }
//             }
//            else if(UVEle->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) 
//             {
//                printf(" second UV \n");
//               int PolygonIndCounter = 0;
//               for(int PolygonId = 0; PolygonId < PolyCount; PolygonId++)
//               {
//                  printf("uv in too 000 \n");
//                  const int PolygonSize = pMesh->GetPolygonSize(PolygonId);
//                  printf("uv in too \n");
//                  for(int VertexIndex = 0; VertexIndex < PolygonSize; VertexIndex++)
//                  {
//                        FbxVector2 uv;

//                        int UVIndex = UseIndex ? UVEle->GetIndexArray().GetAt(PolygonIndCounter) : PolygonIndCounter;
//                       printf("uv in too \n");
//                        uv = UVEle->GetDirectArray().GetAt(UVIndex);

//                        printf(" %ff, %ff, \n", uv.mData[0], 1 - uv.mData[1]);

//                        PolygonIndCounter++;
//                   } 
//               }

//             }
//        //}
// }
 



  //    const int lPolygonCount = pMesh->GetPolygonCount();
  //    printf("\n\n lPolygonCount == %d \n", lPolygonCount);
  //    meshL->PolygonCount = lPolygonCount;
 
  // FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
  //   FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
  //   if (pMesh->GetElementMaterial())
  //   {
  //       lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
  //       lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
  //       if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
  //       {
  //          // FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
  //           if (lMaterialIndice->GetCount() == lPolygonCount)
  //           {
  //               // Count the faces of each material
  //               for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
  //               {
  //                   const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
  //                   if (mSubMeshes.GetCount() < lMaterialIndex + 1)
  //                   {
  //                       mSubMeshes.Resize(lMaterialIndex + 1);
  //                   }
  //                   if (mSubMeshes[lMaterialIndex] == NULL)
  //                   {
  //                       mSubMeshes[lMaterialIndex] = new SubMesh;
  //                   }
  //                   mSubMeshes[lMaterialIndex]->TriangleCount += 1;
  //               }
                
  //               // Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
  //               // if, in the loop above, we resized the mSubMeshes by more than one slot.
  //               for (int i = 0; i < mSubMeshes.GetCount(); i++)
  //               {
  //                   if (mSubMeshes[i] == NULL)
  //                       mSubMeshes[i] = new SubMesh;
  //               }
                
  //               // Record the offset (how many vertex)
  //               const int lMaterialCount = mSubMeshes.GetCount();
  //               int lOffset = 0;
  //               for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
  //               {
  //                   mSubMeshes[lIndex]->IndexOffset = lOffset;
  //                   lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
  //                   // This will be used as counter in the following procedures, reset to zero
  //                   mSubMeshes[lIndex]->TriangleCount = 0;
  //               }
  //              // FBX_ASSERT(lOffset == lPolygonCount * 3);
  //           }
  //       }
  //   }
    
  //   // All faces will use the same material.
  //   if (mSubMeshes.GetCount() == 0)
  //   {
  //       mSubMeshes.Resize(1);
  //       mSubMeshes[0] = new SubMesh();
  //   }
  
 
 
  
  //   hasNormalExists = pMesh->GetElementNormalCount() > 0;
  //   hasUvWxists     = pMesh->GetElementUVCount() > 0;
  //   hasTangentExists = pMesh->GetElementTangentCount() > 0;
  //   hasBiNormalExists = pMesh->GetElementBinormalCount() > 0;
 

  //   // need to indexes correct 


  //   FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
  //   FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
  //   FbxGeometryElement::EMappingMode lTangentMode = FbxGeometryElement::eNone;
  //   FbxGeometryElement::EMappingMode lBiNormalMode = FbxGeometryElement::eNone;
    
  //   if (hasNormalExists)
  //   {
  //       lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
  //       if (lNormalMappingMode == FbxGeometryElement::eNone)
  //       {
  //           hasNormalExists = false;
  //       }
  //       if (hasNormalExists && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
  //       {
  //           mAllByControlPoint = false;
  //       }
  //   }
  //   if (hasUvWxists)
  //   {
  //       lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
  //       if (lUVMappingMode == FbxGeometryElement::eNone)
  //       {
  //           hasUvWxists = false;
  //       }
  //       if (hasUvWxists && lUVMappingMode != FbxGeometryElement::eByControlPoint)
  //       {
  //           mAllByControlPoint = false;
  //       }
  //   }
    
  //   if(hasTangentExists)
  //   {
  //       lTangentMode = pMesh->GetElementTangent(0)->GetMappingMode();
  //       if (lTangentMode == FbxGeometryElement::eNone)
  //       {
  //           hasTangentExists = false;
  //       }
  //       if (hasTangentExists && lTangentMode != FbxGeometryElement::eByControlPoint)
  //       {
  //           mAllByControlPoint = false;
  //       }
  //   }
    
  //   if(hasBiNormalExists)
  //   {
  //       lBiNormalMode = pMesh->GetElementBinormal(0)->GetMappingMode();
  //       if(lBiNormalMode == FbxGeometryElement::eNone)
  //       {
  //           hasBiNormalExists = false;
  //       }
  //       if(hasBiNormalExists && lBiNormalMode != FbxGeometryElement::eByControlPoint)
  //       {
  //           mAllByControlPoint = false;
  //       }
  //   }


  //   int lPolygonVertexCount = pMesh->GetControlPointsCount();

 
  //      // if(!mAllByControlPoint)
  //      // {
  //      //  lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
  //      // }

  //    printf(" lPolygonVertexCount == %d  \n", lPolygonVertexCount); 

  //   meshL->vertexPolygonCount = lPolygonVertexCount;

 
  //  // meshL.hitler[0] = 13.2;
  //  // meshL.hitler[1] = 15.7;
  //  // meshL.hitler[2] = 17.6;
  //  // meshL.hitler[3] = 18.5;
  //  // meshL.hitler[4] = 19.3;
  
  //  const FbxVector4 *lControlPoints = pMesh->GetControlPoints();
   

  //     FbxVector4 lCurrentVertex;
  //     FbxVector4 lCurrentNormal;
  //     FbxVector2 lCurrentUV;
  //     FbxVector4 lCurrentTangents;
  //     FbxVector4 lCurrentBiNormal;



  //     const FbxGeometryElementNormal *lNormalElement = nullptr;
  //     const FbxGeometryElementUV *lUVelement = nullptr;
  //     const FbxGeometryElementTangent *lTangentElement = nullptr;
  //     const FbxGeometryElementBinormal *lBinormalElement = nullptr;
   

  //       meshL->lmVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
  //      // printf(" los lm Vertices == %d \n", meshL->lmVertices);
  //       meshL->lmIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_STRIDE]; // +1  i'm add
  //       printf(" lm Indices  sized == %d  \n", lPolygonCount * TRIANGLE_VERTEX_STRIDE);
 


  //       if(hasNormalExists)
  //       {
  //         lNormalElement = pMesh->GetElementNormal(0);
  //          meshL->lmNormal = new float[lPolygonVertexCount * NORMAL_STRIDE];
           
  //       }
 

  //   FbxStringList lUVNames;
  //   pMesh->GetUVSetNames(lUVNames);
  //   const char * lUVName = nullptr;

  //       if(hasUvWxists)
  //       {

  //          lUVelement = pMesh->GetElementUV(0);
  //          printf("LUBELE == !\n" );
  //          meshL->lmUV = new float[lPolygonVertexCount * UV_STRIDE];
  //          lUVName = lUVNames[0];
  //       }

  //        if(hasTangentExists)
  //        {

  //         meshL->existsTang  = true;
  //         lTangentElement = pMesh->GetElementTangent(0);
  //          meshL->lmTangents = new float[lPolygonVertexCount * NORMAL_STRIDE]; 
  //         }
 
  //       if(hasBiNormalExists)
  //         {
  //           meshL->existsBiNorm = true;
  //           lBinormalElement = pMesh->GetElementBinormal(0);
  //         meshL->lmBiNormal = new float[lPolygonVertexCount * NORMAL_STRIDE];
  //         } 
 

  //      printf("My polygon Vertex !  count == %d \n", meshL->vertexPolygonCount);
  //             lPolygonVertexCount = 256 * 3;
  //       for(int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
  //       {
  //            lCurrentVertex = lControlPoints[lIndex];  // was 242  == lPolygonVertexCount == 242  
  //              // printf(" my vertex value ! == %f \n", static_cast<float>(lCurrentVertex[0]));
  //              // printf(" my vertex value second ! == %f \n", static_cast<float>(lCurrentVertex[1]));
  //              // printf(" my vertex value second fin == %f \n", static_cast<float>(lCurrentVertex[2]));
  //          printf("Number to vertex %d \n", lIndex);
  //           meshL->lmVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
  //           meshL->lmVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
  //           meshL->lmVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
  //           meshL->lmVertices[lIndex * VERTEX_STRIDE + 3] = 1;
            
  //           printf("%ff, %ff, %ff, \n", meshL->lmVertices[lIndex * VERTEX_STRIDE], meshL->lmVertices[lIndex * VERTEX_STRIDE + 1],
  //             meshL->lmVertices[lIndex * VERTEX_STRIDE + 2]);
  //           // printf("meshL->lmVertices[lIndex * VERTEX_STRIDE + 1] == %f \n ", meshL->lmVertices[lIndex * VERTEX_STRIDE + 1]);
  //           printf("TO do \n" );
  //           if(hasNormalExists)
  //           { 
  //              //printf(" in Normals ok ! \n");
  //             int lNormalIndex = lIndex;
  //             if(lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
  //             {
  //                printf("&&&&& \n");
  //               lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
  //                printf("normal to this == %d \n", lNormalIndex);
  //             }
  //             lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
  //             // lNormals !!! 
  //              // printf(" my normal value ! == %f \n", static_cast<float>(lCurrentNormal[0]));
  //              // printf(" my normal value second ! == %f \n", static_cast<float>(lCurrentNormal[1]));
  //              // printf(" my normal value second fin == %f \n", static_cast<float>(lCurrentNormal[2]));
  //         printf("Normal  \n" );
  //           meshL->lmNormal[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
  //           meshL->lmNormal[lIndex * NORMAL_STRIDE+1] = static_cast<float>(lCurrentNormal[1]);
  //           meshL->lmNormal[lIndex * NORMAL_STRIDE+2] = static_cast<float>(lCurrentNormal[2]);

  //             }
 

  //         if(hasUvWxists)
  //         {
  //         //  printf(" in UV ok ! \n");
  //           printf("UV 000 \n" );
  //        int lUVIndex = lIndex;
  //        printf("UV 000  %d \n", lUVIndex );
  //         if(lUVelement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
  //         {
  //           try
  //           {
  //               printf("UV 11111 \n" );
  //           lUVIndex = lUVelement->GetIndexArray().GetAt(lIndex);
  //           }catch(std::exception e)
  //           {

  //   std::cerr << "Exception catched : " << e.what() << '\n';
  //           }
           
  //         }
  //         printf("UV222 \n" );
  //         lCurrentUV = lUVelement->GetDirectArray().GetAt(lUVIndex);

  //           //printf(" my UV value == %f \n", static_cast<float>(lCurrentUV[0]));
  //          // printf(" my UV value == %f \n", static_cast<float>(lCurrentUV[1]));
  //         printf("UV  \n" );
  //            meshL->lmUV[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
  //            meshL->lmUV[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

  //         }

  //         if(hasTangentExists)
  //         {
  //               int lTangentsIndex = lIndex;
  //               if(lTangentElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
  //               {
  //                 lTangentsIndex = lTangentElement->GetIndexArray().GetAt(lIndex);
  //               } 

  //               lCurrentTangents = lTangentElement->GetDirectArray().GetAt(lTangentsIndex);
           
  //               meshL->lmTangents[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentTangents[0]);       
  //               meshL->lmTangents[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentTangents[1]); 
  //               meshL->lmTangents[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentTangents[2]); 
  //         }

  //        // BiNormal ! 
  //         if(hasBiNormalExists)
  //         {

  //          // printf(" Binormal  Exists !  \n");
 
  //             int lBiNormalIndex = lIndex;
  //             if(lBinormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
  //             {
  //               lBiNormalIndex = lBinormalElement->GetIndexArray().GetAt(lIndex);
  //             }

  //             lCurrentBiNormal = lBinormalElement->GetDirectArray().GetAt(lBiNormalIndex);

  //             meshL->lmBiNormal[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentBiNormal[0]);
  //             meshL->lmBiNormal[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentBiNormal[1]);
  //             meshL->lmBiNormal[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentBiNormal[2]);

  //         }

  //       } // final for 
 



  //       printf(" vertex INdex ! \n" );

  //      int lVertexCount = 0;
  //       for(int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
  //       {
  //             printf("lPolygonIndex == %d \n", lPolygonIndex);
  //            int lMaterialIndex = 0;
  //            if(lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
  //            {
  //                lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
  //            }
 
  //              // WHAT IS IT ????? 
              

  //            const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset + mSubMeshes[lMaterialIndex]->TriangleCount * 3;
  //              //printf(" lIndexOffset == %d \n", lIndexOffset);
  //            for(int lVertexIndex = 0; lVertexIndex < TRIANGLE_VERTEX_COUNT; ++lVertexIndex)
  //            {
  //                const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVertexIndex);

  //                  if(mAllByControlPoint)
  //                  {
  //                  meshL->lmIndices[lIndexOffset + lVertexIndex] = static_cast<unsigned int>(lControlPointIndex);
  //                  }
  //                  else 
  //                  {
  //                        // printf( " redraw my values ! %d \n", lIndexOffset + lVertexIndex);
  //                  meshL->lmIndices[lIndexOffset + lVertexIndex] = static_cast<unsigned int>(lVertexCount);
  //                   //  printf(" %d, \n", meshL->lmIndices[lIndexOffset + lVertexIndex]);

  //                       lCurrentVertex = lControlPoints[lControlPointIndex];

  //                             meshL->lmVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
  //                             meshL->lmVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
  //                             meshL->lmVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
  //                             meshL->lmVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;
  //                           printf(" %ff, %ff, %ff, \n", meshL->lmVertices[lVertexCount * VERTEX_STRIDE], meshL->lmVertices[lVertexCount * VERTEX_STRIDE + 1],
  //                           meshL->lmVertices[lVertexCount * VERTEX_STRIDE + 2] );
                   
  //                    if(hasNormalExists)
  //                    {
  //                       pMesh->GetPolygonVertexNormal(lPolygonIndex, lVertexIndex, lCurrentNormal);
  //                  // lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
  //                  // lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
  //                  // lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);

  //                        meshL->lmNormal[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
  //                        meshL->lmNormal[lVertexCount * NORMAL_STRIDE+1] = static_cast<float>(lCurrentNormal[1]);
  //                        meshL->lmNormal[lVertexCount * NORMAL_STRIDE+2] = static_cast<float>(lCurrentNormal[2]);
  //                    printf(" %ff, %ff, %ff, \n", meshL->lmNormal[lVertexCount * VERTEX_STRIDE], meshL->lmNormal[lVertexCount * VERTEX_STRIDE + 1],
  //                           meshL->lmNormal[lVertexCount * VERTEX_STRIDE + 2] );
            

  //                    }


  //                    if(hasUvWxists)
  //                    {
  //                       bool lUnmappedUV;
  //                   pMesh->GetPolygonVertexUV(lPolygonIndex, lVertexIndex, lUVName, lCurrentUV, lUnmappedUV);
  //                   //lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
  //                   //lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

  //                    meshL->lmUV[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
  //                    meshL->lmUV[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);



  //                     printf("  %ff, %ff,  \n",  meshL->lmUV[lVertexCount * UV_STRIDE], meshL->lmUV[lVertexCount * UV_STRIDE + 1]);


  //                    }

  //                  }
  //                  ++lVertexCount;
  //            } 
  //           mSubMeshes[lMaterialIndex]->TriangleCount += 1; 
  //       }

    //    const GLsizei lElementCount = mSubMeshes[pMaterial]->TriangleCount * 3;
    //     const int lElementCount = mSubMeshes[pMaterial]->TriangleCount * 3;
    //     printf(" lElementCount == %d  \n", lElementCount);
    //      const int countSubMesh = mSubMeshes.GetCount();
    // for(int a = 0; a < countSubMesh; ++a)
    // {
    //    size_t triangFinalElements = mSubMeshes[a]->TriangleCount * 3;
    //    printf(" triangFinalElements == %zu  \n", triangFinalElements);
    // }
        // printf(" countSubMesh == %d  \n", countSubMesh);
         

  return false;
}


void LoadCacheRecursive(FbxScene* lScene, LosBestMesh *meshL)
{

       const int lTextureCount = lScene->GetTextureCount();

       printf(" lTextureCount == %d \n", lTextureCount);

       // LoadCacheRecurThree(lScene->GetRootNode());
 
  LoadCacheRecurThree(lScene->GetRootNode(), meshL); 
} 




void LoadCacheRecurThree(FbxNode *pNode, LosBestMesh *meshL)
{
 
   FbxNodeAttribute *lNodeAttribute = pNode->GetNodeAttribute();
   printf(" NodeAttribute %d  \n", pNode->GetNodeAttributeCount());

  if(lNodeAttribute)
  {

     printf("my node is ! \n");

       if(lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
       {
            FbxMesh *lMesh = pNode->GetMesh();
            if(lMesh && !lMesh->GetUserDataPtr())
            {
               printf(" in VBO \n");
                 if(InitializeFbxMesh(lMesh, meshL))
                 {
                     printf("ok !\n");
                 }
            }
       }
  }

   const int lChildCount = pNode->GetChildCount();
   printf(" lChildCount %d \n", lChildCount);

   for(int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
   {
       LoadCacheRecurThree(pNode->GetChild(lChildIndex), meshL);
   }

}
 

void PrintTangents(const LosBestMesh *meshTanget)
{
     
  printf("\nmy Tangents: \n\n");
for(int i = 0; i < meshTanget->vertexPolygonCount; ++i)
   {

printf("%ff, %ff, %ff, \n", meshTanget->lmTangents[i * NORMAL_STRIDE], 
  meshTanget->lmTangents[i * NORMAL_STRIDE + 1], meshTanget->lmTangents[i * NORMAL_STRIDE + 2] );
   }
}


void PrintBiNormals(const LosBestMesh *meshBiNornals)
{

   printf("\nmy BiNormals: \n\n");
for(int i = 0; i < meshBiNornals->vertexPolygonCount; ++i)
   {

      printf("%ff, %ff, %ff, \n", meshBiNornals->lmBiNormal[i * NORMAL_STRIDE],
       meshBiNornals->lmBiNormal[i * NORMAL_STRIDE + 1], meshBiNornals->lmBiNormal[i * NORMAL_STRIDE  + 2] );
    
   }
}



int dim4 = 4;
int dim6 = 6;

   
int main (int argc, char** argv )       // was const char* argv[] 
{ 
  

     // EXAMPLE 
    //TODO:: second parametr fbx or gltf
    // TODO ::   work fain ./convertFBX_toLosMesh '/Users/sergeiloskutnikov/Desktop/Goodness/game_model/Aqsinesy Love/' coin_final.fbx
  bool losFbx = false;

  //   int (rLs)[dim6][dim4]; 
 
  // printf(" my rLs == %d \n", rLs);

AnimTrue = false;

  //   oc.add_operation(std::shared_ptr<Operation>(new Operation_Double_Sum()));
 // std::shared_ptr<LosBestMesh> ls_Mesh(new LosBestMesh("LosSer "));

  // was LosMesh
    auto *lsMesh = new LosBestMesh();

  g_program_name = argv[0];

   int aee = argc;

   printf(" today we are ! %d  \n", aee);

    for( int i=0; i < aee; i++)
    {
        printf(" my string == %s \n", argv[i]);
    }


      if(argc > 2)
      {
 
     std::string fileExtension  = std::string(argv[argc-1]);

     losNameMesh = std::string(argv[argc-2]);
     losOutputMesh = std::string(argv[argc-1]);
      
       printf(" file extension == %s \n", fileExtension.c_str());
 
      auto const exten = fileExtension.find_last_of('.');
      const auto leaf = fileExtension.substr(exten + 1);
      std::string newRl= fileExtension.substr(exten + 1);


      std::cout << leaf << '\n';
     
    
    std::string glf = "gltf";
 std::cout << " newRl = " << newRl << '\n' ;


     if( (newRl.compare(glf)) == 0)
     {
    std::cout << " my string GLTF " << '\n';
        LoadGLTF(losNameMesh, losOutputMesh);
     }
     else
     {
      losFbx = true;
    std::cout << " my string fbx " << '\n';
       LoadFBX(losNameMesh, losOutputMesh, lsMesh);
     }


      } 
      else if (argc == 1)
        {
          return 0;
        }
    


   //     float *lmVertices = nullptr;
   // unsigned int *lmIndices = nullptr; 
   // float *lmNormal  = nullptr;
   // float *lmUV = nullptr;
   // float *lmTangents = nullptr; 
   // float *lmBiNormal = nullptr;
   //   int vertexPolygonCount; 
   // int normalCount;
   // int uvCount;
   // int tangentCount;
   // int biNormalCount;


//       printf(" vertexPolygonCount los == %d \n", lsMesh->vertexPolygonCount);
//  printf(" normalCount los == %d \n", lsMesh->normalCount);
//  printf(" uvCount los == %d \n", lsMesh->uvCount);
//  printf(" tangentCount los == %d \n", lsMesh->tangentCount);
// printf(" biNormalCount los == %d \n", lsMesh->biNormalCount);
 

//    if(losFbx)
//    {
  
//    const int verC = lsMesh->vertexPolygonCount * 4;
//  printf("\nmy Vertex: count == %d \n\n", verC);
//    for(int i = 0; i < lsMesh->vertexPolygonCount * 4; ++i)
//    {
//   printf("%ff, %ff, %ff, %ff,\n",  lsMesh->lmVertices[i * VERTEX_STRIDE], lsMesh->lmVertices[i * VERTEX_STRIDE + 1], lsMesh->lmVertices[i * VERTEX_STRIDE + 2],
//        lsMesh->lmVertices[i * VERTEX_STRIDE + 3]);
      
//    }


// printf("\n my Normal: \n\n");
// for(int i = 0; i < lsMesh->vertexPolygonCount * 3; ++i)
//    {

//   printf("%ff, %ff, %ff,\n",  lsMesh->lmNormal[i * NORMAL_STRIDE],  lsMesh->lmNormal[i * NORMAL_STRIDE + 1],  lsMesh->lmNormal[i * NORMAL_STRIDE + 2]); 

//    }

// printf("\nmy Uv: \n\n");
// for(int i = 0; i < lsMesh->vertexPolygonCount * 2; ++i)
//    {

//  printf("%ff, %ff,\n",  lsMesh->lmUV[i * UV_STRIDE], lsMesh->lmUV[i * UV_STRIDE + 1]);
    
//    }



//  const int losCountNumber = lsMesh->PolygonCount * 3;

//    printf(" \n my Indices: %d \n\n", losCountNumber);

//    for(int q = 0; q < losCountNumber; ++q)
//    {
//     printf(" %d, %d, %d, \n", lsMesh->lmIndices[q * NORMAL_STRIDE], lsMesh->lmIndices[q * NORMAL_STRIDE + 1], lsMesh->lmIndices[q * NORMAL_STRIDE + 2] );
//    }

   // printf(" \n my indices \n\n");
   // const int coiunt = lsMesh->PolygonCount * 3;
   // for(int d = 0; d < coiunt; ++d)
   // {
   //    printf ("%u, ", lsMesh->lmIndices[d]);
   // }

if(lsMesh->existsTang && lsMesh->existsBiNorm)
{

    //printf("\n Tangent & BiNormal is ok \n");
   // PrintTangents(lsMesh);
   // PrintBiNormals(lsMesh);

}








//    printf("my Tang: \n\n");
// for(int i = 0; i < lsMesh->vertexPolygonCount; ++i)
//    {

    
//    }

//    printf("my BiNorm: \n\n");
// for(int i = 0; i < lsMesh->vertexPolygonCount; ++i)
//    {

    
//    }





  //delete lsMesh;

     // float *exampleU = new float[lsMesh.vertexPolygonCount * 4];
     // exampleU = lsMesh.lmVertices;

     // printf(" ser FLOAT !== %f \n", exampleU[1000]);
 
    // *** final file 

   // printf(" my outPut path == %s \n", losOutputMesh.c_str());



    // FILE *fileL = fopen(losOutputMesh.c_str(), "w");
  
    //   if (fileL == nullptr) {
    //   //log_.Log(kLogError, "Could not open %s for writing\n", file_name.c_str());
    //     printf("Fucks ! \n");
    //   return 0;
    // }

  //    char buffer[] = { 'w' , 't' , 'f' };
  // fwrite(buffer , 1 , sizeof(buffer) , ptrFile ); // записать в файл содержимое буфера
  // fclose (ptrFile);
   
    //  char  bufL[]  = {'a', 'b'};
   //callToWrite
 


//  struct LosVecFour
// {
//    float l1;
//    float l2;
//    float l3;
// };
 
// struct FContainer
// {
//     FHeader header;
//     FData data;
//     LosVecFour vectLos;
   //  LosVecFour vectLos[2];
// };

 

    FContainer in_container;
    // FContainer of_container;
    // of_container.header.signature = 'R';
    // of_container.header.version = 0x1;
    // of_container.data.number = 5;
    // of_container.data.sign = 'A';
    // of_container.data.state = false;
    // of_container.vectLos[1].l1 = 45.0f;
    // of_container.vectLos[1].l2 = 123.04f;
    // of_container.vectLos[1].l3 = -3.034f;

    // of_container.vectLos[2].l3 = -0.49f;

    std::vector<Deve> stSDe;
 
//    Deve devOnes; 
//    devOnes.serv[0] = -1.3452f;
//    devOnes.serv[1] =  0.932f;
//    devOnes.serv[2] = -0.00312f;
// stSDe.push_back(devOnes);

 
//      std::ofstream output_stream("fileF.bin", std::ios::binary);
//     // output_stream.write(reinterpret_cast<const char*>(&of_container), sizeof(of_container));
//     // output_stream.close();

//      output_stream.write((const char*)&stSDe.at(0).serv, sizeof(float) * 3);
//      output_stream.close();



// struct Deve 
// {
//   float serv[3];
//   float roi[3];
// };

    
 //      Deve verPut;

 //    std::ifstream input_stream("file.bin", std::ios::binary);
 //    input_stream.read((char*)&verPut.serv, sizeof(float) * 3);
 //    stSDe.push_back(verPut);
 // //    input_stream.read(reinterpret_cast<char*>(&in_container), sizeof(in_container));
 //    input_stream.close();
 
 //     std::cout << stSDe.at(0).serv[1] << '\n';
 //    std::cout << in_container.header.version << '\n';
 //    std::cout << in_container.data.number << '\n';
 //    std::cout << in_container.data.sign << '\n';
 //    std::cout << in_container.data.state << '\n';
 //     std::cout << in_container.vectLos[1].l1 << '\n';
 //  std::cout << in_container.vectLos[2].l3 << '\n';

     
     int awe = always();
 
   printf(" my %d \n", awe);

   //  callToWrite(losOutputMesh.c_str());
 

    // Write the binary data to the file and close it.
    // TODO: Add option to write json file too.
    // log_.Log(kLogVerbose, "Writing %s\n", file_name.c_str());
    // fwrite(fbb.GetBufferPointer(), 1, fbb.GetSize(), file);
 
       // ls_Mesh  was bufL
      // fwrite(bufL, 1, sizeof(bufL), fileL);
 
     //fwrite(&ls_Mesh, 1, sizeof(LosBestMesh), fileL);


  // working version  == 

 //    int noiK = 443;
 //  Example_cpp(noiK);
 
 //  printf(" my const char * %s \n", fplutil::AxisSystemNames());
 //    fplutil::todayA(noiK);
    

 //    // lsMesh.losVect.push_back(45.9);
 //    // lsMesh.losVect.push_back(99.0);
  
 // // newLL mmwl;
 // // mmwl.vertexPolygonCount = 303;
 
 //  printf("my name ExampleNN == %s \n", nameExampleN_files.c_str());
  
// int ou = 120;

//  lsMesh.serExal = &ou;

//   std::ofstream fileRead;
//    fileRead.open(nameExampleN_files.c_str(), std::ios::out | std::ios::binary);   // std::ofstream::out

//    std::streampos size;

//     size = sizeof(lsMesh);
   
//     std::cout << " size == " << size << std::endl;

//      fileRead.write((char*)&lsMesh, size);

//    fileRead.close();
  



    //newLL llN;



// LosBestMesh lBackMesh;

// std::ifstream myfile;
//   myfile.open(nameExampleN_files.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
 
//     if(myfile)
//     {
//       printf(" aaa  \n");
//     }
//     else
//     {
//       printf(" bbbb \n");
//     }

//    std::streampos begin = myfile.tellg();
  
//    std::cout << " my size 1 == " << begin << std::endl;
 
//     myfile.seekg(0, std::ios::beg);
//    myfile.read((char*)&lBackMesh, begin);  
    
//     printf("in pre out !\n");
//     myfile.close();
 
 
   //float *hoiu = new float[lBackMesh.vertexPolygonCount * 4];
     //hoiu = lBackMesh.lmVertices;

 //  printf(" my int  ! %d \n", (int*)&lBackMesh.serExal);

 // printf(" my vertexPolygonCount == %d \n ", lBackMesh.vertexPolygonCount);
    
     // for( int oop = 0; oop < 5; oop++)
     // {
       //printf(" my vertex float  == %d \n ", lBackMesh.losVect.at(1));
     // } 

     //  for(auto const& value : lBackMesh.losVect) {
    //      std::cout << " my vector value == " << value << std::endl;
    //  }
 
  // delete hoiu;


   // streampos begin,end;
  // ifstream myfile ("example.bin", ios::binary);
  // begin = myfile.tellg();
  // myfile.seekg (0, ios::end);
  // end = myfile.tellg();
  // myfile.close();
  // cout << "size is: " << (end-begin) << " bytes.

     //std::streampos end = myfile.tellg();
    //std::cout << " my size 2 == " << end << std::endl;

// fclose(fileL);
 
     // if(losFbx)
     // {

   //       float *lmVertices = nullptr;
   // unsigned int *lmIndices = nullptr; 
   // float *lmNormal  = nullptr;
   // float *lmUV = nullptr;
   // float *lmTangents = nullptr; 
   // float *lmBiNormal = nullptr;

   // int *serExal;
    
         delete [] lsMesh->lmBiNormal;
         delete [] lsMesh->lmTangents;
         delete [] lsMesh->lmUV;
         delete [] lsMesh->lmNormal;
         delete [] lsMesh->lmVertices;
         delete [] lsMesh->lmIndices;

  std::cout << '\n' << " Success load 3D model !" << '\n'; 
//}
    return 0;

}
