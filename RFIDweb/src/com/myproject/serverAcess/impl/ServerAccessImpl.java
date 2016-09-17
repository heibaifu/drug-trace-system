package com.myproject.serverAcess.impl;


/*
 * web�������ӿ��ָࣺ��url ��  ������ʹ��BufferedReader �������ݡ�
 *  
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import com.myproject.serverAcess.ServerAccess;
public class ServerAccessImpl implements ServerAccess{
	public ServerAccessImpl() {
		super();
		// TODO Auto-generated constructor stub
	}
	public String dopost(String table,Double tagid ) throws Exception{
		HttpURLConnection httpConn = null;
		String path = "http://192.168.191.1:8080/RFIDserver/tag?table="+table+"&tagid="+tagid;
		BufferedReader in=null;
		try {
            URL url=new URL(path);
            httpConn=(HttpURLConnection)url	.openConnection();
            //��ȡ��Ӧ
            if(httpConn.getResponseCode()==HttpURLConnection.HTTP_OK){
                StringBuffer content=new StringBuffer();
                String tempStr="";
                in=new BufferedReader(new InputStreamReader(httpConn.getInputStream(),"gb2312"));
                while((tempStr=in.readLine())!=null){
                    content.append(tempStr);
                }
                return content.toString();
            }else{
                throw new Exception("�������������!");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }finally{
            in.close();
            httpConn.disconnect();
        }
		return null;
	}
}
