package com.example.rfid;

import java.util.concurrent.ExecutionException;

import httpapi.HttpTask;
import ui.AnimationTask;
import ui.MyProcess;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	TextView tv;
	EditText et1,et2;
	String tosearch;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		tv=(TextView)findViewById(R.id.textView2);
		et1=(EditText)findViewById(R.id.table);
		et2=(EditText)findViewById(R.id.tag);
		
		
	}
	
	public void search(View v)//�����ʾ��TextView���������������Ӧ��
	{
		
		
		double tagid=Double.valueOf(et2.getText().toString());
		String table=et1.getText().toString();
		HttpTask task=new HttpTask(tv,table,tagid);//�����첽�̣߳�������������
		task.execute("ee");
		try {
			tosearch=task.get();//��ȡ��������ؼ���Ϣ
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ExecutionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void broswer(View v)//��������ؼ���Ϣ
	{
		Uri uri=Uri.parse("http://www.baidu.com/s?ie=UTF-8&wd="+tosearch);
		Intent i=new Intent(Intent.ACTION_VIEW,uri);
		startActivity(i);
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
