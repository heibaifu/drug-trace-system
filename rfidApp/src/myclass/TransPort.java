package myclass;

public class TransPort {
	public int transId;
	public String tagId;
	public String startTime;
	public String endTime;
	public String transportStatus;	
	public String transportName;
	
	
	public int getTransId() {
		return transId;
	}
	public void setTransId(int transId) {
		this.transId = transId;
	}
	public String getEndTime() {
		return endTime;
	}
	public void setEndTime(String endTime) {
		this.endTime = endTime;
	}

	public String getTransportName() {
		return transportName;
	}
	public void setTransportName(String transportName) {
		this.transportName = transportName;
	}
	
	public String getTagId() {
		return tagId;
	}
	public void setTagId(String tagId) {
		this.tagId = tagId;
	}
	public String getStartTime() {
		return startTime;
	}
	public void setStartTime(String startTime) {
		this.startTime = startTime;
	}
	public String getTransportStatus() {
		return transportStatus;
	}
	public void setTransportStatus(String transportStatus) {
		this.transportStatus = transportStatus;
	}
	public String getTransportResult()
	{
		String result="TagID:"+this.tagId+"\n"+"������:"+this.transportName+"\n"
				+"���俪ʼ:"+this.startTime+"\n"+"�������:"+this.endTime+"\n"+
				"����״̬:"+this.transportStatus+"\n";
		return result;
	}
	
}
