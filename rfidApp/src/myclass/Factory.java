package myclass;

public class Factory {
	public String factoryId;
	public String factoryName;
	public String outputTime;
	public String product_line1_status;
	
	public String product_line2_status;
	public String leave_status;
	public String Tagid;
	
	
	
	
	
	public String getTagid() {
		return Tagid;
	}
	public void setTagid(String tagid) {
		Tagid = tagid;
	}
	public String getFactoryId() {
		return factoryId;
	}
	public void setFactoryId(String factoryId) {
		this.factoryId = factoryId;
	}
	public String getFactoryName() {
		return factoryName;
	}
	public void setFactoryName(String factoryName) {
		this.factoryName = factoryName;
	}
	public String getOutputTime() {
		return outputTime;
	}
	public void setOutputTime(String outputTime) {
		this.outputTime = outputTime;
	}
	public String getProduct_line1_status() {
		return product_line1_status;
	}
	public void setProduct_line1_status(String product_line1_status) {
		this.product_line1_status = product_line1_status;
	}
	public String getProduct_line2_status() {
		return product_line2_status;
	}
	public void setProduct_line2_status(String product_line2_status) {
		this.product_line2_status = product_line2_status;
	}
	public String getLeave_status() {
		return leave_status;
	}
	public void setLeave_status(String leave_status) {
		this.leave_status = leave_status;
	}
	public String getFactoryResult()
	{
		String result="";
		result="TagID="+this.Tagid+"\n"+
		"��������:"+this.factoryName+"\n"+
		"��������:"+this.outputTime+"\n"+
		"��Ʒ��1:"+this.product_line1_status+"\n"+
		"��Ʒ��2:"+this.product_line2_status+"\n"+
		"����״̬:"+this.leave_status+"\n";
		return result;
	}
	
}
