using UnityEngine;  
using System.Collections;  
using UnityEngine.UI;  
//摄像机  陀螺仪转动  
public class TGyro : MonoBehaviour  
{  
	
	//bool gyroBool;  
	Gyroscope gyro;  
	Quaternion quatMult;  
	Quaternion quatMap;  
	//UILabel ul;  
	public GameObject player;  
	public GameObject camParent;  
	void Awake()  
	{  
		//player = GameObject.Find("Player");  
		// find the current parent of the camera's transform  
		Transform currentParent = transform.parent;  
		// instantiate a new transform  
		camParent = new GameObject("camParent");  
		// match the transform to the camera position  
		camParent.transform.position = transform.position;  
		// make the new transform the parent of the camera transform  
		transform.parent = camParent.transform;  
		// make the original parent the grandparent of the camera transform  
		//camParent.transform.parent = currentParent;-  
		// instantiate a new transform  
		GameObject camGrandparent = new GameObject("camGrandParent");  
		// match the transform to the camera position  
		camGrandparent.transform.position = transform.position;  
		// make the new transform the parent of the camera transform  
		camParent.transform.parent = camGrandparent.transform;  
		// make the original parent the grandparent of the camera transform  
		camGrandparent.transform.parent = currentParent;  
		
		//gyroBool = true;  
		//if (gyroBool) {  
		gyro = Input.gyro;  
		//ul = GameObject.Find("Rotation").GetComponent<UILabel>();  
		
		gyro.enabled = true;  
		camParent.transform.eulerAngles = new Vector3(0,0, 0);  
		quatMult = new Quaternion(0, 0, 1, 0);//猜测陀螺仪默认是向下的，这里是沿z轴转180度  
		
	}  
	
	void Update()  
	{  
		
		quatMap = new Quaternion(gyro.attitude.x, gyro.attitude.y, gyro.attitude.z, gyro.attitude.w);  
		Quaternion qt=quatMap * quatMult;  
		
		transform.localRotation =qt;  
		
		
		//ul.text = "Rotation:" + transform.localRotation + "player" + player.transform.localRotation;  
		
		
	}  
	
}  