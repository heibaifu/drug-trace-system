package myclass;

public class Store {
	public int storeId;
	public String soldTime;
	public String storeName;

	public String tagId;
	public String stockTime;
	public String storeStatus;
	
	
	public String getTagId() {
		return tagId;
	}
	public void setTagId(String tagId) {
		this.tagId = tagId;
	}
	public int getStoreId() {
		return storeId;
	}
	public void setStoreId(int storeId) {
		this.storeId = storeId;
	}
	public String getSoldTime() {
		return soldTime;
	}
	public void setSoldTime(String soldTime) {
		this.soldTime = soldTime;
	}
	public String getStoreName() {
		return storeName;
	}
	public void setStoreName(String storeName) {
		this.storeName = storeName;
	}
	public String getStockTime() {
		return stockTime;
	}
	public void setStockTime(String stockTime) {
		this.stockTime = stockTime;
	}
	public String getStoreStatus() {
		return storeStatus;
	}
	public void setStoreStatus(String storeStatus) {
		this.storeStatus = storeStatus;
	}
	public String getStoreResult()
	{
		String result="TagID:"+this.tagId+"\n"+
				"�̵���:"+this.storeName+"\n"+
				"���ʱ��:"+this.stockTime+"\n"+
				"�۳�ʱ��:"+this.soldTime+"\n"+
				"�̵����:"+this.storeStatus+"\n";
				
				return result;
	}
	
	
}
