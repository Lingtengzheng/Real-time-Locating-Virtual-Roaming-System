using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Net.Sockets;


public class client : MonoBehaviour
{
	const int portNo = 9999;
	private TcpClient _client;
	byte[] data;
	string nickName = "";
	string message = "";
	string sendMsg = "";
	Vector3 V1 = new Vector3 (0,50,393); 
	Vector3 V2 = new Vector3 (3,50,393); 
	
	/*public bikecontrol bike_speed_control;*/
	public GameObject MainCamera; 
	public GameObject thing; 
	
	void Start () {
		this._client = new TcpClient();
		this._client.Connect("192.168.1.102", portNo);
		data = new byte[this._client.ReceiveBufferSize];
		//SendMessage(txtNick.Text);
		NetworkStream netstream = _client.GetStream();//提供用于访问网络的基本数据流
		//向服务器发送用户名以确认身份
		
		//string Name="Client";
		
		//netstream.Write(Encoding.Unicode.GetBytes(Name), 0, Encoding.Unicode.GetBytes(Name).Length);
		this._client.GetStream().BeginRead(data, 0, System.Convert.ToInt32(this._client.ReceiveBufferSize), ReceiveMessage, null);
	}
	
	// Update is called once per frame
	void Update () {
		MainCamera.transform .position  =V1;
		thing.transform .position  =V2;
	}
	
	/*void OnGUI()
	{
		//nickName = GUI.TextField(new Rect(10, 10, 100, 20), nickName);
		//message = GUI.TextArea(new Rect(10, 40, 300, 200), message);
		//sendMsg = GUI.TextField(new Rect(10, 250, 210, 20), sendMsg);
		if (GUI.Button(new Rect(120, 10, 80, 20), "Connect"))
		{
			//Debug.Log("hello");

		};
		if (GUI.Button(new Rect(230, 250, 80, 20), "Send"))
		{
			SendMessage(sendMsg);
			sendMsg = "";
		};
	}*/
	
	public void SendMessage(string message)
	{
		try
		{
			NetworkStream ns = this._client.GetStream();
			byte[] data = System.Text.Encoding.ASCII.GetBytes(message);
			ns.Write(data, 0, data.Length);
			ns.Flush();
		}
		catch (Exception ex)
		{
			//MessageBox.Show(ex.ToString());
		}
	}
	public void ReceiveMessage(IAsyncResult ar)
	{
		
		//Debug.Log ("asdsad");
		try
		{
			int bytesRead;
			byte[] buffer = new byte[100];
			//byte[] speed = new byte[8];
			
			this._client.GetStream().Read(buffer, 0, buffer.Length);

			int i =0;
			int n=0;

			string v = "";
			while (true)
			{
				if (buffer[i] == ' ')
				{
					n++;
					if (n == 1)
					{
						//Debug.Log (v);

						V1.x = float.Parse(v);
						v = "";
					}
					if (n == 2)
					{
						//Debug.Log (v);
						
						V1.y = float.Parse(v);
						v = "";
					}
					if(n == 3)
					{
						V1.z = float.Parse(v);
						v = "";
					}
					if (n == 4)
					{
						//Debug.Log (v);
						
						V2.x = float.Parse(v);
						v = "";
					}
					if (n == 5)
					{
						//Debug.Log (v);
						
						V2.y = float.Parse(v);
						v = "";
					}
					if(n == 6)
					{
						V2.z = float.Parse(v);
						v = "";
						break;
					}
				}
				v+= (char)buffer[i];
				i++;
			}
			Debug.Log (V1);
			Debug.Log (V2);
			//Debug.Log (V);

			
			bytesRead = this._client.GetStream().EndRead(ar);
			if (bytesRead < 1)
			{
				return;
			}
			
			this._client.GetStream().BeginRead(data, 0, System.Convert.ToInt32(this._client.ReceiveBufferSize), ReceiveMessage, null);
			
			
		}
		catch (Exception ex)
		{
		}
	}
}