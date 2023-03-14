#include "Framework.h"
#include "Model.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Model::Model()
{
}

Model::~Model()
{
    for (ModelBone* bone : bones)
        SafeDelete(bone);

    for (ModelMesh* mesh : meshes)
        SafeDelete(mesh);

    for (Material* material : materials)
        SafeDelete(material);

    for (ModelClip* clip : clips)
        SafeDelete(clip);
}

void Model::ReadMesh(wstring file)
{
    file =  L"../../_Models/" + file + L".mesh";

    BinaryReader* r = new BinaryReader(file);

    //Load Bone
    UINT count = 0;
    count = r->UInt();
    for (UINT i = 0; i < count; i++)
    {
       ModelBone* bone = new ModelBone();

       bone->index = r->Int();
       bone->name = String::ToWString(r->String());
       bone->parentIndex = r->Int();
       bone->transform = r->Matrix();

        bones.push_back(bone);
    }

    //Load Mesh
    count = r->UInt();
    for (UINT i = 0; i < count; i++)
    {
        ModelMesh* mesh = new ModelMesh();
        
        mesh->boneIndex = r->Int();

        //Vertex Data
        {
            UINT count = r->UInt();

            vector<Model::VertexModel> vertices;
            vertices.assign(count, Model::VertexModel());

            void* ptr = (void*)&vertices[0];
            r->Byte(&ptr, sizeof(Model::VertexModel) * count);

            mesh->vertices = new Model::VertexModel[count];
            mesh->vertexCount = count;
            copy
            (
                vertices.begin(), vertices.end(),
                stdext::checked_array_iterator<Model::VertexModel*>(mesh->vertices, count)
            );
        }

        //Index Data
        {
            UINT count = r->UInt();

            vector<UINT> indices;
            indices.assign(count, UINT());

            void* ptr = (void*)&indices[0];
            r->Byte(&ptr, sizeof(UINT) * count);

            mesh->indices = new UINT[count];
            mesh->indexCount = count;
            copy
            (
                indices.begin(), indices.end(),
                stdext::checked_array_iterator<UINT*>(mesh->indices, count)
            );
        }

        UINT partCount = r->UInt();
        for (UINT k = 0; k < partCount; k++)
        {
            ModelMeshPart* meshPart = new ModelMeshPart();
            meshPart->materialName = String::ToWString(r->String());

            meshPart->startVertex = r->UInt();
            meshPart->vertexCount = r->UInt();

            meshPart->startIndex = r->UInt();
            meshPart->indexCount = r->UInt();

            mesh->meshParts.push_back(meshPart);
        }

        meshes.push_back(mesh);
    }

    SafeDelete(r);

    BindBone();
}

void Model::ReadMaterial(wstring file)
{
    file = L"../../_Textures/" + file + L".material";

    Xml::XMLDocument* document = new Xml::XMLDocument();
    Xml::XMLError error = document->LoadFile(String::ToString(file).c_str());
    assert(error == Xml::XML_SUCCESS);

    Xml::XMLElement* root = document->FirstChildElement();
    Xml::XMLElement* materialNode = root->FirstChildElement();

    do
    {
        Material* material = new Material();

        Xml::XMLElement* node = nullptr;

        node = materialNode->FirstChildElement();
        material->Name(String::ToWString(node->GetText()));

        wstring directory = Path::GetDirectoryName(file); 
        String::Replace(&directory, L"../../_Textures", L"");

        wstring texture = L"";

        node = node->NextSiblingElement();
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->DiffuseMap(directory + texture);

        node = node->NextSiblingElement();
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->SpecularMap(directory + texture);

        node = node->NextSiblingElement();
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->NormalMap(directory + texture);

        Color color;

        node = node->NextSiblingElement();
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Ambient(color);

        node = node->NextSiblingElement();
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Diffuse(color);

        node = node->NextSiblingElement();
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Specular(color);

        node = node->NextSiblingElement();
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Emissive(color);

        materials.push_back(material);

        materialNode = materialNode->NextSiblingElement();
    }
    while (materialNode != nullptr);

    BindMesh();
}

void Model::ReadClip(wstring file)
{
    file = L"../../_Models/" + file + L".clip";

    BinaryReader* r = new BinaryReader(file);

    ModelClip* clip = new ModelClip();
    clip->name = String::ToWString(r->String());
    clip->frameRate = r->Float();
    clip->frameCount = r->UInt();

    UINT keyframeCount = r->UInt();
    for (UINT i = 0; i < keyframeCount; i++)
    {
        ModelKeyFrame* keyframe = new ModelKeyFrame();
        keyframe->BoneName = String::ToWString(r->String());

        UINT size = r->UInt();
        if (size > 0)
        {
            keyframe->Transforms.assign(size, ModelKeyFrameData());

            void* ptr = (void*)&keyframe->Transforms[0];
            r->Byte(&ptr, sizeof(ModelKeyFrameData) * size);
        }

        clip->keyFrameMap[keyframe->BoneName] = keyframe;
    }

    SafeDelete(r);

    clips.push_back(clip);

}

void Model::BindBone()
{
    root = bones[0];

    for (ModelBone* bone : bones)
    {
        if (bone->parentIndex > -1)
        {
            bone->parent = bones[bone->parentIndex];
            bone->parent->children.push_back(bone);
        }
        else
            bone->parent = nullptr;
    }
}

void Model::BindMesh()
{
    for (ModelMesh* mesh : meshes)
    {
        mesh->bone = bones[mesh->boneIndex];
        mesh->Binding(this);
    }
}

ModelBone* Model::BoneByName(wstring name)
{
    for (ModelBone* bone : bones)
    {
        if (name == bone->Name())
            return bone;
    }

    return nullptr;
}

Material* Model::MaterialByName(wstring name)
{
    for (Material* material : materials)
    {
        if (name == material->Name())
            return material;
    }

    return nullptr;
}

ModelClip* Model::ClipByName(wstring name)
{
    for (ModelClip* clip : clips)
    {
        if (name == clip->name)
            return clip;
    }

    return nullptr;
}
