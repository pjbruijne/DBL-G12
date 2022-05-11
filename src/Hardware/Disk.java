package Hardware;
public class Disk {

    private int color; // 0 = white, 1 = black
    private float diameter = 2.3f; // in cm
    private float height = 1f; // in cm

    public Disk(int color) {
        this.color = color;
    }

    public int getColor() {
        return color;
    }

    public float getDiameter() {
        return diameter;
    }

    public float getHeight() {
        return height;
    }
}
