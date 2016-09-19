package httpapi;

import java.io.IOException;
import android.os.AsyncTask;
import android.widget.TextView;

public class HttpTask extends AsyncTask<String, String, String> {

	HttpUtils util;
	TextView tv;
	String tab;
	public HttpTask(TextView v,String table,double tagId)
	{
		tv=v;tab=table;
		util=new HttpUtils(table,tagId);
		
	}
	@Override
	protected String doInBackground(String... params) {
		
		String a="";
		try {
			a = util.getinputResult();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String b=util.getResult(a);
		System.out.println(b);
		
		publishProgress(b);//����ui�̣߳�����ѯ�������ύ��ui�߳�
		String s="";
		if(b.equals("���޴˽��"))
		{
			return "û�н��";
		}
		else{
		if(tab.equals("tag"))
		{
			s=b.split("\n")[1];
			s=s.replaceAll("medicineName:","" );
			return s; //�������������Ϣ������Ҫ����Ϣ��ͨ���ٶȿ��Բ�ѯ
		
		}
		if(tab.equals("factory"))
		{
			s=b.split("\n")[1];
			s=s.replaceAll("��������:", "");
			return s; //�������������Ϣ������Ҫ����Ϣ��ͨ���ٶȿ��Բ�ѯ
		}
		if(tab.equals("store"))
		{
			s=b.split("\n")[1];
			s=s.replaceAll("ҩ����:","" );
			return s; //�������������Ϣ������Ҫ����Ϣ��ͨ���ٶȿ��Բ�ѯ
		
		}
		if(tab.equals("tag"))
		{
			s=b.split("\n")[1];
			s=s.replaceAll("������:","" );
			return s;           //�������������Ϣ������Ҫ����Ϣ��ͨ���ٶȿ��Բ�ѯ
		
		}
		}
		return b;
	}

	@Override
	protected void onPreExecute() {
		// TODO Auto-generated method stub
		super.onPreExecute();
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
	}

	@Override
	protected void onProgressUpdate(String... values) {
		// TODO Auto-generated method stub
		String a=values[0];		//����ui�̸߳��첽�̵߳Ľӿڣ�ui������������û�ֱ�ۿɼ���Ч�����������ǽ����������ʾ����Ļ��

		tv.setText(a);
		super.onProgressUpdate(values);
	}
	
}
