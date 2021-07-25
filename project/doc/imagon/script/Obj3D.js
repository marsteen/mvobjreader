//---------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------

var OBJ_FLAG_TEXT = 1;
var OBJ_FLAG_NORM = 2;
var OBJ_FLAG_MAT  = 4;

var Obj3D_Textures = [];


//---------------------------------------------------------------------------
//
// class:       
// function:    CreateTexture
//
//
//---------------------------------------------------------------------------


function CreateTexture(textureFile)
{
	var Texture = gl.createTexture();
	
	Texture.image = new Image();

	Texture.image.onload = function()
	{
		handleLoadedTexture(Texture);
	}
	Texture.image.src = textureFile;
	return Texture;
}


//---------------------------------------------------------------------------
//
// Klasse:     Obj3D_Group
// Methode:    Group_Draw
//
//---------------------------------------------------------------------------

function Group_Draw()
{
  
  if (this.mMaterial.mHasTexture)
  { 
    
    gl.bindTexture(gl.TEXTURE_2D, this.mMaterial.mTexture);
    /*
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, this.mMaterial.mTexture);
    gl.uniform1i(shaderProgram3d.samplerUniform, 0);
    */
		gl.uniform1i(shaderProgram3d.useTexture, 1);
  }
  else
  {
		gl.uniform1i(shaderProgram3d.useTexture, 0);
    gl.uniform1f(shaderProgram3d.materialTranspUniform, 0.5); //this.mMaterial.mTransp); 
		gl.uniform3f(shaderProgram3d.materialColorUniform, this.mMaterial.mDiffuse[0], this.mMaterial.mDiffuse[1], this.mMaterial.mDiffuse[2]);
	}

  

	//
	// Vertices
	//

	gl.bindBuffer(gl.ARRAY_BUFFER, this.mVertexBuffer);
	gl.vertexAttribPointer(shaderProgram3d.vertexPositionAttribute, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(shaderProgram3d.vertexPositionAttribute);	

	//
	// Textur-Koordinaten
	//
		
	if (this.mFlags & 	OBJ_FLAG_TEXT)
	{	
	  
	  gl.bindBuffer(gl.ARRAY_BUFFER, this.mTexCoordBuffer);
	  gl.vertexAttribPointer(shaderProgram3d.textureCoordAttribute, 2, gl.FLOAT, false, 0, 0);
	  gl.enableVertexAttribArray(shaderProgram3d.textureCoordAttribute);
	}
	else
	{
	  gl.disableVertexAttribArray(shaderProgram3d.textureCoordAttribute);
	}
	
	//
	// Normalen
	//

	if (this.mFlags & 	OBJ_FLAG_NORM)
	{	
    gl.bindBuffer(gl.ARRAY_BUFFER, this.mNormalBuffer);
    gl.vertexAttribPointer(shaderProgram3d.vertexNormalAttribute, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(shaderProgram3d.vertexNormalAttribute);
  }
  else
  {
    gl.disableVertexAttribArray(shaderProgram3d.vertexNormalAttribute);
  }
    
	gl.drawArrays(gl.TRIANGLES, 0, this.mFaceCount * 3);  
}

//---------------------------------------------------------------------------
//
// Klasse:     Obj3D_Group
// Methode:    Group_Draw
//
//---------------------------------------------------------------------------

function Obj3D_Group()
{
  this.Draw = Group_Draw;
}


//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    CreateGroup
//
//---------------------------------------------------------------------------

function DebugVar(name, v)
{
  document.write(name);
  document.write(v);
  document.write("<br>");
}


//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    CreateGroup
//
//---------------------------------------------------------------------------

function CreateGroup(obj, index)
{
  var Group = new Obj3D_Group();
  
  if (obj[index++] = "GRP")
  {
    Group.mName      = obj[index++];
    Group.mMatName   = obj[index++];
    Group.mFlags     = parseInt(obj[index++]);
    Group.mFaceCount = parseInt(obj[index++]);
    Group.mVertCount = Group.mFaceCount * 3; 
    Group.mVertices  = new Float32Array(Group.mVertCount * 3);
    
/*
    document.write("  Facecount=");
    document.write(Group.mFaceCount);
    document.write("<br>");    

    document.write("  mMatName=");
    document.write(Group.mMatName);
    document.write("<br>");    
    
    document.write("  mName=");
    document.write(Group.mName);
    document.write("<br>");
*/    

    var j = 0;
    for (var i = 0; i < Group.mVertCount; i++)
    {
      Group.mVertices[j++] = obj[index++];
      Group.mVertices[j++] = obj[index++];
      Group.mVertices[j++] = obj[index++];
    }
    
    Group.mVertexBuffer   = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Group.mVertexBuffer);	
    gl.bufferData(gl.ARRAY_BUFFER, Group.mVertices, gl.STATIC_DRAW);
	  
    
    if (Group.mFlags & OBJ_FLAG_TEXT)
    {
      this.mTexCoordBuffer = gl.createBuffer(); 
      
      Group.mTexCoords = new Float32Array(Group.mVertCount * 2);

      j = 0;      
      for (var i = 0; i < Group.mVertCount; i++)
      {      
        Group.mTexCoords[j++] = obj[index++]; 
        Group.mTexCoords[j++] = obj[index++];
      }
      
      Group.mTexCoordBuffer   = gl.createBuffer();
      gl.bindBuffer(gl.ARRAY_BUFFER, Group.mTexCoordBuffer);	
      gl.bufferData(gl.ARRAY_BUFFER, Group.mTexCoords, gl.STATIC_DRAW);
    }
    
    if (Group.mFlags & OBJ_FLAG_NORM)
    {
      
      /*
      document.write("  Facecount=");
      document.write(Group.mFaceCount);
      document.write("<br>");
      */

      Group.mNormals = new Float32Array(Group.mVertCount * 3);

      j = 0;      
      for (var i = 0; i < Group.mVertCount; i++)
      {      
        Group.mNormals[j++] = obj[index++];
        Group.mNormals[j++] = obj[index++];
        Group.mNormals[j++] = obj[index++];          
      }

      Group.mNormalBuffer   = gl.createBuffer();
      gl.bindBuffer(gl.ARRAY_BUFFER, Group.mNormalBuffer);	
      gl.bufferData(gl.ARRAY_BUFFER, Group.mNormals, gl.STATIC_DRAW);
	      
    }    
  }
  else
  {
    document.write("GRP Kennung fehlt *****");
    document.write("<br>");
  }
  
  
  return Group;
}

//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    GetGroupSize
//
//---------------------------------------------------------------------------


function GetGroupSize(obj, index)
{
  var GroupSize = 5;
  var Flags     = parseInt(obj[index + 3]);
  var FaceCount = parseInt(obj[index + 4]);
  
  GroupSize += FaceCount * 3 * 3;
  
  if (Flags & OBJ_FLAG_TEXT)
  {
    GroupSize += FaceCount * 2 * 3;
  }
  
  if (Flags & OBJ_FLAG_NORM)
  {
    GroupSize += FaceCount * 3 * 3;
  }
  return GroupSize;
  
}



//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    CreateMaterial
//
//---------------------------------------------------------------------------

function CreateMaterial(obj, index)
{
  var Material = new Object();
  
  if (obj[index++] = "MAT")
  {
    Material.mDiffuse  = new Array();
    Material.mAmbient  = new Array();
    Material.mEmission = new Array();
    Material.mSpecular = new Array();
    
    Material.mName        = obj[index++];
    Material.mTexName     = obj[index++];
    Material.mNs          = obj[index++];
    Material.mTransp      = obj[index++];    
    Material.mIllum       = obj[index++];
    
    Material.mDiffuse[0]  = obj[index++];
    Material.mDiffuse[1]  = obj[index++];
    Material.mDiffuse[2]  = obj[index++];

    Material.mAmbient[0]  = obj[index++];
    Material.mAmbient[1]  = obj[index++];
    Material.mAmbient[2]  = obj[index++];

    Material.mEmission[0] = obj[index++];
    Material.mEmission[1] = obj[index++];
    Material.mEmission[2] = obj[index++];

    Material.mSpecular[0] = obj[index++];
    Material.mSpecular[1] = obj[index++];
    Material.mSpecular[2] = obj[index++];
    
    if (Material.mTexName != "")
    {
      if (Obj3D_Textures[Material.mTexName] == undefined)
      {
        var NewTex = new CreateTexture(Material.mTexName);
        Obj3D_Textures[Material.mTexName] = NewTex; 
        Material.mTexture = NewTex;
      }
      else
      {
        Material.mTexture = Obj3D_Textures[Material.mTexName]; 
      }
      
      Material.mHasTexture = true;    
    }
    else
    {
      Material.mHasTexture = false;
    }
      
    
  }
  else
  {
    document.write("MAT Kennung fehlt *****!<br>");
  }
  
  
  return Material;
}

//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    Obj3D_Draw
//
//---------------------------------------------------------------------------

function Obj3D_Draw(obj)
{
  for (var g = 0; g < this.mGroupCount; g++)
  {
    this.mGroups[g].Draw();
  }
}

//---------------------------------------------------------------------------
//
// Klasse:     Obj3D
// Methode:    Obj3D
//
//---------------------------------------------------------------------------


function Obj3D(obj)
{
  var index = 0;
  if (obj[index++] == "IMA3D")
  {
    
    this.Draw = Obj3D_Draw;
    
    
    this.mMaterialCount = obj[index++];
    this.mGroupCount    = obj[index++];
    
    
    this.mGroups    = [];
    this.mMaterials = [];
    
    for (var m = 0; m < this.mMaterialCount; m++)
    {
       var Material = CreateMaterial(obj, index);
       this.mMaterials[Material.mName] = Material;
       index += 18;      
    }
    
    for (var g = 0; g < this.mGroupCount; g++)
    {
       var Group = CreateGroup(obj, index);
       Group.mMaterial = this.mMaterials[Group.mMatName];
       
       this.mGroups.push(Group);
       index += GetGroupSize(obj, index);
    }    
  } 
  else
  {
     document.write("IMA3D Kennung fehlt *****");
     document.write(obj[0]);
  }
}

