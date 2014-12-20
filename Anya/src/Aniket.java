/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * 
 * @author Rakesh
 */
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.ButtonGroup;
import java.lang.Object;
import java.math.BigDecimal;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import oracle.spatial.geometry.JGeometry;
import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import javax.swing.SwingUtilities;
import oracle.sql.STRUCT;

 
public class Aniket extends javax.swing.JFrame {

    /**
     * Creates new form Aniket
     */
    
    populate pop;
    Connection con;
    Statement s;
    ResultSet rs;
    int x;
    int y;
    int r;
    String rstring="";
    int highx;
    int highy;
    int highr;
    int[] rx=new int[50];
    int[] ry=new int[50];
    int rcount=0;
    STRUCT st1;
    String Broken;
    int px,py;
    int pointx;
    int pointy;
    String[] query=new String[50];
    String mainq=new String();
    int count=1;
    
    public class Name
    {
        
    }
    public Aniket() {
        this.pop = new populate();
  
       initComponents();
       groupButton();
       con=pop.connect();
       if(con==null)
       {
           System.out.println("wrong");
       }
       count=0;
       
      }
    ///WHOLE STUDENTS
    public void all(){
        
        
        try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
             rs = s.executeQuery("SELECT scoordinates from students");
            query[count]="SELECT scoordinates from students";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            // System.out.println("do");
             
             Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(new Color(1,255,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs.next()){
           
              st1 = (oracle.sql.STRUCT) rs.getObject(1);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 10, 10);
              
               }
             
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
    }
    
    //WHOLE ASYSTEM
    public void allsystem()
    {
     
        try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
            ResultSet rs1 = s.executeQuery("SELECT position,radius from asystem");
            query[count]="SELECT position,radius from asystem";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
             Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(new Color(255,1,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs1.next()){
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              r=rs1.getInt(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 15, 15);
               gd.drawOval(x-r, y-r,r*2, r*2);
             
              
               }
             
              
         
 
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
    }
    
    
    //WHOLE BUILDINGS
    public void allbuildings()
    {
      int a[]= new int[50];
      int b[]=new int[50];
      int vertices;
      double[] d=new double[50];
        try{
             s=con.createStatement();
           // s.executeQuery("select * from students");
            
            ResultSet rs2 = s.executeQuery("SELECT coordinates,vertices from buildings");
            query[count]="SELECT coordinates,vertices from buildings";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st3;
             Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(new Color(255,255,0,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs2.next()){
           
              st3 = (oracle.sql.STRUCT) rs2.getObject(1);
              vertices=rs2.getInt(2);
             
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st3);
               d=jgeom.getOrdinatesArray();
               
               for(int j=0,i=0;i<d.length;i++)
               {
                   if(i%2==0)
                   {
                       a[j]=(int)d[i];
                       j++;
                   }
               }
                for(int k=0,i=0;i<d.length;i++)
               {
                   if(i%2!=0)
                   {
                       b[k]=(int)d[i];
                       k++;
                   }
               }
               gd.drawPolyline(a, b, vertices);
               
             // System.out.println(d[0]+" "+d[1]);
             
              
               }  
            
               }
          catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
        
    
    
    }
    //POINT QUERY
    public void pointquery(int x,int y)
    {
        px=x;
        py=y;
        Graphics2D gd= (Graphics2D) jLabel1.getGraphics();
        gd.setColor(Color.red);
        gd.fillRect(px, py, 5, 5);
        gd.drawOval(px-50, py-50, 100, 100);
        r=50;
    }
    public void afterpoint()
    {
           Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             
       //students
        try{
        s=con.createStatement();
       ResultSet rs2 = s.executeQuery("SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'");
        query[count]="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
//String check="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_FILTER(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
       // System.out.println(check);
        rs2.next();
      
         
             gd.setColor(new Color(1,255,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
             while(rs2.next()){
           
              st1 = (oracle.sql.STRUCT) rs2.getObject(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 10, 10);
            }
        }
        catch(Exception e)
        {
            System.out.println("pahua");
            e.printStackTrace();
        }
        try{
            rs = s.executeQuery("SELECT c.scoordinates,SDO_NN_DISTANCE(1) dist  FROM students c  WHERE SDO_NN(c.scoordinates,sdo_geometry(2001, NULL, sdo_point_type("+px+","+py+",NULL), NULL,NULL),  'sdo_num_res=1',1) = 'TRUE' ORDER BY dist");
             query[count]="SELECT c.scoordinates,SDO_NN_DISTANCE(1) dist  FROM students c  WHERE SDO_NN(c.scoordinates,sdo_geometry(2001, NULL, sdo_point_type("+px+","+py+",NULL), NULL,NULL),  'sdo_num_res=1',1) = 'TRUE' ORDER BY dist";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            String check="SELECT c.person_id  FROM students c  WHERE SDO_NN(c.scoordinates,sdo_geometry(2001, NULL, sdo_point_type(10,7,NULL), NULL,NULL),  'sdo_num_res=1') = 'TRUE'";
            //System.out.println(check);
            gd.setColor(new Color(255,255,1,255));
           rs.next();
           
            st1 = (oracle.sql.STRUCT) rs.getObject(1);
             BigDecimal b;
            b = (BigDecimal)rs.getObject(2);
           b = b.round(new MathContext(4, RoundingMode.HALF_UP));
           
           
           double d1= b.doubleValue();
           if(d1<50.00)
           {
           JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();            
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 10, 10);
           
            
        }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        
              
        
        //asystem
        {
        try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
            ResultSet rs1 = s.executeQuery("SELECT c.position,c.as_id FROM asystem c WHERE SDO_INSIDE(c.position, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'");
            query[count]="SELECT c.position,c.as_id FROM asystem c WHERE SDO_INSIDE(c.position, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
             
             gd.setColor(new Color(255,255,1,255));
             
            
               while(rs1.next()){
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 15, 15);
              
             
              
               }
             
              
         
 
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
    
        }
        //buildings
        {
        int a[]= new int[50];
      int b[]=new int[50];
      int vertices;
      double[] d=new double[50];
        try{
             s=con.createStatement();
           // s.executeQuery("select * from students");
            
            ResultSet rs2 = s.executeQuery("SELECT c.coordinates,c.vertices FROM buildings c WHERE SDO_ANYINTERACT(c.coordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'");
            query[count]="SELECT c.coordinates,c.vertices FROM buildings c WHERE SDO_ANYINTERACT(c.coordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st3;
             
             gd.setColor(new Color(255,255,0,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs2.next()){
           
              st3 = (oracle.sql.STRUCT) rs2.getObject(1);
              vertices=rs2.getInt(2);
             
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st3);
               d=jgeom.getOrdinatesArray();
               
               for(int j=0,i=0;i<d.length;i++)
               {
                   if(i%2==0)
                   {
                       a[j]=(int)d[i];
                       j++;
                   }
               }
                for(int k=0,i=0;i<d.length;i++)
               {
                   if(i%2!=0)
                   {
                       b[k]=(int)d[i];
                       k++;
                   }
               }
               gd.drawPolyline(a, b, vertices);
               
             // System.out.println(d[0]+" "+d[1]);
             
              
               }  
            
               }
          catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
    
         }
    
    
    }
    
    ///HIGHLIGHT AS
    public void highlight(int hx,int hy)
    {
        Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
        gd.setColor(Color.yellow);
        gd.fillRect(hx, hy, 5, 5);
         try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
            ResultSet rs1 = s.executeQuery("SELECT c.position,c.radius  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+hx+","+hy+",NULL), NULL,NULL),  'sdo_num_res=1') = 'TRUE'");
            query[count]="SELECT c.position,c.radius  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+hx+","+hy+",NULL), NULL,NULL),  'sdo_num_res=1') = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
           
             gd.setColor(new Color(255,1,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs1.next()){
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              r=rs1.getInt(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
              double[] d=jgeom.getPoint();
           
             
               x=(int) d[0];
               y=(int) d[1];
              // System.out.println("ASYSTEM AT "+x+" "+y+" IS BROKEN");
              gd.fillRect(x, y, 15, 15);
               gd.drawOval(x-r, y-r,r*2, r*2);
               highx=x;
               highy=y;
               highr=r;
             
              
               }
             
              
         
 
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        } 
      }
    //RANGE
    public void range(int irx,int iry)
    {
         Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
        gd.setColor(new Color(0,255,255,255));
        gd.fillRect(irx, iry, 5, 5);
         rx[rcount]=irx;
         ry[rcount]=iry;
         System.out.println(rx[rcount]+" "+ry[rcount]);
         rstring+=rx[rcount]+","+ry[rcount]+",";
         if(rcount>0)
         {
             gd.drawLine(rx[rcount-1], ry[rcount-1], rx[rcount], ry[rcount]);
         }
         rcount++;
        
    }
    //RANGE
    public void rquery()
    {
        
        //STUDENTS
        Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
        try{
        s=con.createStatement();
       ResultSet rs2 = s.executeQuery("SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'");
       query[count]="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'";
           count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count]; 
       String check="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'";
            System.out.println(check);

        
      
         
             gd.setColor(new Color(1,255,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
             while(rs2.next()){
           
              st1 = (oracle.sql.STRUCT) rs2.getObject(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 10, 10);
            }
        }
        catch(Exception e)
        {
            System.out.println("pahua");
            e.printStackTrace();
        }
        
        //ASYSTEM
        try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
            ResultSet rs1 = s.executeQuery("SELECT c.position ,c.radius FROM asystem c WHERE SDO_INSIDE(c.position,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'");
            query[count]="SELECT c.position ,c.radius FROM asystem c WHERE SDO_INSIDE(c.position,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
             //Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(new Color(255,1,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs1.next()){
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              r=rs1.getInt(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 15, 15);
              // gd.drawOval(x-r, y-r,r*2, r*2);
             
              
               }
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
        
        //BUILDINGS
        {
        int a[]= new int[50];
      int b[]=new int[50];
      int vertices;
      double[] d=new double[50];
        try{
             s=con.createStatement();
           // s.executeQuery("select * from students");
            
            ResultSet rs2 = s.executeQuery("SELECT c.coordinates ,c.vertices FROM buildings c WHERE SDO_ANYINTERACT(c.coordinates,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'");
            query[count]="SELECT c.coordinates ,c.vertices FROM buildings c WHERE SDO_ANYINTERACT(c.coordinates,SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+rstring+"))) = 'TRUE'";
            count++;
            mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st3;
           //  Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(new Color(255,255,0,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs2.next()){
           
              st3 = (oracle.sql.STRUCT) rs2.getObject(1);
              vertices=rs2.getInt(2);
             
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st3);
               d=jgeom.getOrdinatesArray();
               
               for(int j=0,i=0;i<d.length;i++)
               {
                   if(i%2==0)
                   {
                       a[j]=(int)d[i];
                       j++;
                   }
               }
                for(int k=0,i=0;i<d.length;i++)
               {
                   if(i%2!=0)
                   {
                       b[k]=(int)d[i];
                       k++;
                   }
               }
               gd.drawPolyline(a, b, vertices);
               
             // System.out.println(d[0]+" "+d[1]);
             
              
               }  
            
               }
          catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
    } 
        rcount=0;
        rstring="";
        for(int i=0;i<rx.length;i++)
        {
            rx[i]=0;
            ry[i]=0;
        }
    }
    //EMERGENCY
    public void emergency(int ex,int ey)
    {
        //FIND THE BROKEN ASYSTEM
        Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
        gd.setColor(Color.ORANGE);
        gd.fillRect(ex, ey, 5, 5);
         try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            
            ResultSet rs1 = s.executeQuery("SELECT c.position,c.radius,c.as_id  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+ex+","+ey+",NULL), NULL,NULL),  'sdo_num_res=1') = 'TRUE'");
            query[count]="SELECT c.position,c.radius,c.as_id  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+ex+","+ey+",NULL), NULL,NULL),  'sdo_num_res=1') = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
           
             
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               rs1.next();
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              r=rs1.getInt(2);
              Broken =rs1.getString(3);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
              double[] d=jgeom.getPoint();
           
             
               x=(int) d[0];
               y=(int) d[1];
               gd.setColor(getcolor(rs1.getString(3)));
               System.out.println("ASYSTEM AT "+x+" "+y+" IS BROKEN WITH NAME "+Broken);
              gd.fillRect(x, y, 15, 15);
               gd.drawOval(x-r, y-r,r*2, r*2);
               highx=x;
               highy=y;
               highr=r;
            
              
                
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        } 
    
    


     }
    public Color getcolor(String s)
    {
        
    if(s.equals("a1psa"))
        return Color.LIGHT_GRAY;
    if(s.equals("a3sgm"))
        return Color.BLUE;
    if(s.equals("a4hnb"))
        return Color.MAGENTA;
    if(s.equals("a5vhe"))
        return Color.WHITE;
    if(s.equals("a6ssc"))
        return Color.PINK;
    if(s.equals("a7helen"))
        return new Color(223,20,191,255);
    if(s.equals("a1psa"))
        return new Color(239,228,179,255);
    
    else
        return new Color(145,56,87,255);
    
    }
    
  
    
    
    

    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        buttonGroup2 = new javax.swing.ButtonGroup();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        jCheckBox1 = new javax.swing.JCheckBox();
        jCheckBox2 = new javax.swing.JCheckBox();
        jCheckBox3 = new javax.swing.JCheckBox();
        jLabel3 = new javax.swing.JLabel();
        jRadioButton2 = new javax.swing.JRadioButton();
        jRadioButton3 = new javax.swing.JRadioButton();
        jRadioButton4 = new javax.swing.JRadioButton();
        jRadioButton5 = new javax.swing.JRadioButton();
        jRadioButton6 = new javax.swing.JRadioButton();
        jLabel4 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();
        jButton2 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/map.jpg"))); // NOI18N
        jLabel1.setCursor(new java.awt.Cursor(java.awt.Cursor.CROSSHAIR_CURSOR));
        jLabel1.setInheritsPopupMenu(false);
        jLabel1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jLabel1MouseClicked(evt);
            }
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jLabel1MousePressed(evt);
            }
        });
        jLabel1.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                jLabel1MouseMoved(evt);
            }
        });

        jLabel2.setText("Active Feature Type");

        jCheckBox1.setText("AS");
        jCheckBox1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox1ActionPerformed(evt);
            }
        });

        jCheckBox2.setText("Buildings");
        jCheckBox2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox2ActionPerformed(evt);
            }
        });

        jCheckBox3.setText("Students");
        jCheckBox3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox3ActionPerformed(evt);
            }
        });

        jLabel3.setText("Query");

        jRadioButton2.setLabel("Whole Region");
        jRadioButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton2ActionPerformed(evt);
            }
        });

        jRadioButton3.setLabel("Point Query");
        jRadioButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton3ActionPerformed(evt);
            }
        });

        jRadioButton4.setLabel("Range Query");
        jRadioButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton4ActionPerformed(evt);
            }
        });

        jRadioButton5.setLabel("Surrounding Student");
        jRadioButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton5ActionPerformed(evt);
            }
        });

        jRadioButton6.setLabel("Emergency Query");
        jRadioButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton6ActionPerformed(evt);
            }
        });

        jLabel4.setText(" ");
        jLabel4.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                jLabel4MouseMoved(evt);
            }
        });

        jLabel5.setText("Coordinates");

        jButton1.setText("SUBMIT   QUERY");
        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton1MouseClicked(evt);
            }
        });
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jTextArea1.setColumns(20);
        jTextArea1.setRows(5);
        jScrollPane1.setViewportView(jTextArea1);

        jButton2.setText("CLEAR");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 833, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPane1)
                        .addGap(13, 13, 13)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jCheckBox1)
                    .addComponent(jCheckBox2)
                    .addComponent(jCheckBox3)
                    .addComponent(jLabel3)
                    .addComponent(jRadioButton2)
                    .addComponent(jRadioButton3)
                    .addComponent(jRadioButton4)
                    .addComponent(jRadioButton5)
                    .addComponent(jRadioButton6)
                    .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, 102, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel5, javax.swing.GroupLayout.PREFERRED_SIZE, 102, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jButton1, javax.swing.GroupLayout.DEFAULT_SIZE, 136, Short.MAX_VALUE)
                    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(0, 27, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 34, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jCheckBox1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jCheckBox2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jCheckBox3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton5)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton6)
                        .addGap(37, 37, 37)
                        .addComponent(jLabel5)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(51, 51, 51)
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 82, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 70, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jLabel1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 87, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jLabel1.getAccessibleContext().setAccessibleDescription("");

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jRadioButton6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton6ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jRadioButton6ActionPerformed

    private void jRadioButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton3ActionPerformed
        // TODO add your handling code here:
        jLabel1.repaint();
        
        
    }//GEN-LAST:event_jRadioButton3ActionPerformed

    private void jLabel4MouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel4MouseMoved

        // TODO add your handling code here:
    }//GEN-LAST:event_jLabel4MouseMoved

    private void jLabel1MouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel1MouseMoved
        // TODO add your handling code here:
        jLabel4.setText(Integer.toString(evt.getX())+" "+Integer.toString(evt.getY()));
    }//GEN-LAST:event_jLabel1MouseMoved

    private void jRadioButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton2ActionPerformed
        // TODO add your handling code here:
     
        
       
    }//GEN-LAST:event_jRadioButton2ActionPerformed

    private void jCheckBox3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox3ActionPerformed
        // TODO add your handling code here:
      
                
        
        
    }//GEN-LAST:event_jCheckBox3ActionPerformed

    private void jCheckBox1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox1ActionPerformed
        // TODO add your handling code here:
      
       
        
    }//GEN-LAST:event_jCheckBox1ActionPerformed

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        // TODO add your handling code here:
        jTextArea1.setText(mainq); 
      
        //wholeregion
        if(jRadioButton2.isSelected())
              {
                  if(jCheckBox3.isSelected())
                      all();
                  if(jCheckBox1.isSelected())
                       allsystem();
                  if(jCheckBox2.isSelected())
                       allbuildings();
                      
              }
        
         if(jRadioButton4.isSelected())
             rquery();
        if(jRadioButton5.isSelected())
        {
           Graphics2D gd= (Graphics2D) jLabel1.getGraphics();
        gd.setColor(Color.red);
       
        
        
       //students
        try{
        s=con.createStatement();
       ResultSet rs2 = s.executeQuery("SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+highx+","+(highy+highr)+","+(highx+highr)+","+highy+","+highx+","+(highy-highr)+"))) = 'TRUE'");
        query[count]="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+highx+","+(highy+highr)+","+(highx+highr)+","+highy+","+highx+","+(highy-highr)+"))) = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
//String check="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_FILTER(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
       // System.out.println(check);
            rs2.next();
      
         
             gd.setColor(new Color(1,255,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
             while(rs2.next()){
           
              st1 = (oracle.sql.STRUCT) rs2.getObject(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x=(int) d[0];
               y=(int) d[1];
              gd.fillRect(x, y, 10, 10);
            }
        }
        catch(Exception e)
        {
            System.out.println("pahua");
            e.printStackTrace();
        }
        }
        if(jRadioButton6.isSelected())
        {
           Graphics2D gd= (Graphics2D) jLabel1.getGraphics();
        gd.setColor(Color.red);
       
        
        
       //students
        try{
        s=con.createStatement();
       ResultSet rs2 = s.executeQuery("SELECT * FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+highx+","+(highy+highr)+","+(highx+highr)+","+highy+","+highx+","+(highy-highr)+"))) = 'TRUE'");
       query[count]="SELECT * FROM students c WHERE SDO_INSIDE(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+highx+","+(highy+highr)+","+(highx+highr)+","+highy+","+highx+","+(highy-highr)+"))) = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count]; 
//String check="SELECT c.person_id ,c.scoordinates FROM students c WHERE SDO_FILTER(c.scoordinates, SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY("+px+","+(py+r)+","+(px+r)+","+py+","+px+","+(py-r)+"))) = 'TRUE'";
       // System.out.println(check);
         
         rs2.next();
        
      int x[]=new int[50];
      int y[]=new int[50];
      int ecount=0;
      
         
             gd.setColor(new Color(1,255,1,255));
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
             while(rs2.next())
             {
                 
              st1 = (oracle.sql.STRUCT) rs2.getObject(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st1);
              double[] d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               x[ecount]=(int) d[0];
               y[ecount]=(int) d[1];
               
              gd.fillRect(x[ecount], y[ecount], 10, 10);
              ecount++;
            // rs5 = s.executeQuery("SELECT c.position,c.radius  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+x+","+y+",NULL), NULL,NULL),  'sdo_num_res=2') = 'TRUE'");
             // while(rs.next())
              {
                 // System.out.println(rs.getInt(2));
             }
              
             }
             ecount=0;
            for(int i=0;i<x.length;i++)
             {
                 // System.out.println(x[i]+" "+y[i]);
             if(x[i]==0 && y[i]==0)
             {
             break;}
             else
             {
             rs = s.executeQuery("SELECT c.position,c.radius,c.as_id  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+x[i]+","+y[i]+",NULL), NULL,NULL),  'sdo_num_res=2') = 'TRUE'");
             query[count]="SELECT c.position,c.radius,c.as_id  FROM asystem c  WHERE SDO_NN(c.position,sdo_geometry(2001, NULL, sdo_point_type("+x[i]+","+y[i]+",NULL), NULL,NULL),  'sdo_num_res=2') = 'TRUE'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count]; 
             int next=0;
             String newas="";
             rs.next();
             int previousint=rs.getInt(2);
             String previousString=rs.getString(3);
             while(rs.next())
             {
            // System.out.println(rs.getInt(2));
             next=rs.getInt(2); 
             newas=rs.getString(3);
             }
             if(Broken.equals(newas))
             {
                 newas=previousString;
             }
             Color c;
             c=getcolor(newas);
             System.out.println(newas+" "+next+" "+ previousint+" ");
             gd.setColor(c);
             gd.fillRect(x[i],y[i], 10, 10);
             
             int a[]= new int[50];
             int b[]=new int[50];
              int vertices;
              int m,n;
              double[] d=new double[50];
            String check="SELECT position,radius from asystem WHERE as_id='"+newas+"'";
            System.out.println(check);
             try{
             s=con.createStatement();
            // s.executeQuery("select * from students");
            newas=newas.trim();
            
            ResultSet rs1 = s.executeQuery("SELECT position,radius from asystem WHERE as_id='"+newas+"'");
            
            query[count]="SELECT position,radius from asystem WHERE as_id='"+newas+"'";
            count++;
           mainq = mainq+"\nQuery: "+count+" "+query[--count];
            
             STRUCT st2;
           //  Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
             gd.setColor(c);
             
             //STRUCT st = (oracle.sql.STRUCT) rs.getObject(1);
               while(rs1.next()){
           
              st2 = (oracle.sql.STRUCT) rs1.getObject(1);
              r=rs1.getInt(2);
             // System.out.print("d");
              JGeometry jgeom = JGeometry.load(st2);
               d=jgeom.getPoint();
             // System.out.println(d[0]+" "+d[1]);
               m=(int) d[0];
               n=(int) d[1];
              gd.fillRect(m, n, 15, 15);
               gd.drawOval(m-r, n-r,r*2, r*2);
             
              
               }
             
             
              
         
 
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
            System.out.print("Statement problem");
        }
             } 
             
             }
             
             
             
        }
        catch(Exception e)
        {
            System.out.println("pahua");
            e.printStackTrace();
        }
        
        }
            
        
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jCheckBox2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox2ActionPerformed
        // TODO add your handling code here:
      
    }//GEN-LAST:event_jCheckBox2ActionPerformed
   
    private void jLabel1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel1MouseClicked
        // TODO add your handling code here:
         
        if(jRadioButton5.isSelected())
             highlight(evt.getX(),evt.getY());
      if(jRadioButton3.isSelected())
             pointquery(evt.getX(),evt.getY());
      if(jRadioButton6.isSelected())
             emergency(evt.getX(),evt.getY());
      
             
        
    }//GEN-LAST:event_jLabel1MouseClicked

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        // TODO add your handling code here:
        jLabel1.repaint();
        jTextArea1.setText(" ");
        mainq=" ";
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jRadioButton5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton5ActionPerformed
        // TODO add your handling code here:
        
            
    }//GEN-LAST:event_jRadioButton5ActionPerformed

    private void jRadioButton4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton4ActionPerformed
        // TODO add your handling code here:
        
    }//GEN-LAST:event_jRadioButton4ActionPerformed

    private void jLabel1MousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel1MousePressed
        // TODO add your handling code here:
         if(SwingUtilities.isRightMouseButton(evt)&&(jRadioButton4.isSelected()))
                {
                    Graphics2D gd=(Graphics2D) jLabel1.getGraphics();
                    gd.setColor(new Color(0,255,255,255));
                    
                    gd.drawLine(rx[0], ry[0], rx[rcount-1], ry[rcount-1]);
                    rstring=rstring.substring(0,rstring.length()-1);
                 //   System.out.println(rstring);
                   // System.out.println(rx[0]+" "+ry[0]);
                }
                if(SwingUtilities.isLeftMouseButton(evt)&&(jRadioButton4.isSelected()))
                {
                 range(evt.getX(),evt.getY());
                    
                }
    }//GEN-LAST:event_jLabel1MousePressed

    private void jButton1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseClicked
        // TODO add your handling code here:
        if(jRadioButton3.isSelected())
            afterpoint();
        
        
    }//GEN-LAST:event_jButton1MouseClicked
private void groupButton (){
    ButtonGroup bg1= new ButtonGroup();
    
    bg1.add(jRadioButton2);
   bg1.add(jRadioButton3);
   bg1.add(jRadioButton4);
   bg1.add(jRadioButton5);
   bg1.add(jRadioButton6);
}
    /**
     * @param args the command line arguments
     */


    public static void main(String args[])  {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Aniket.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Aniket.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Aniket.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Aniket.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
     
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Aniket().setVisible(true);
            }
        });
        
        
        
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.ButtonGroup buttonGroup2;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JCheckBox jCheckBox2;
    private javax.swing.JCheckBox jCheckBox3;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JRadioButton jRadioButton2;
    private javax.swing.JRadioButton jRadioButton3;
    private javax.swing.JRadioButton jRadioButton4;
    private javax.swing.JRadioButton jRadioButton5;
    private javax.swing.JRadioButton jRadioButton6;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTextArea jTextArea1;
    // End of variables declaration//GEN-END:variables
}
