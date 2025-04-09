import React, { useState, useEffect, useRef } from "react";
import { View, Text, TouchableOpacity, StyleSheet, FlatList, Alert } from "react-native";
import DateTimePickerModal from "react-native-modal-datetime-picker";
import Ionicons from "react-native-vector-icons/Ionicons";
import { db, ref, set, get } from "../firebase";

export default function ScheduleScreen() {
  const [schedules, setSchedules] = useState([]);
  const [showPicker, setShowPicker] = useState(false);
  const [editIndex, setEditIndex] = useState(null);
  const [selectedDate, setSelectedDate] = useState(null);
  const executedTimes = useRef(new Set());

  useEffect(() => {
    const fetchSchedules = async () => {
      try {
        const schedulesRef = ref(db, "schedules");
        const snapshot = await get(schedulesRef);
        if (snapshot.exists()) {
          const data = snapshot.val();
          const sortedSchedules = Object.values(data).sort((a, b) => new Date(a) - new Date(b));
          setSchedules(sortedSchedules);
        }
      } catch (error) {
        console.error("Error fetching schedules:", error);
      }
    };
    fetchSchedules();
  }, []);

  useEffect(() => {
    const interval = setInterval(() => {
      const now = new Date();
      const nowRounded = new Date(now.getFullYear(), now.getMonth(), now.getDate(), now.getHours(), now.getMinutes());

      schedules.forEach((isoStr) => {
        const scheduledTime = new Date(isoStr);
        const scheduledRounded = new Date(scheduledTime.getFullYear(), scheduledTime.getMonth(), scheduledTime.getDate(), scheduledTime.getHours(), scheduledTime.getMinutes());

        if (nowRounded.getTime() === scheduledRounded.getTime() && !executedTimes.current.has(isoStr)) {
          handleFeedFood();
          executedTimes.current.add(isoStr);
        }

        console.log("Now:", nowRounded.getTime());
        console.log("Scheduled:", scheduledRounded.getTime());
      });
    }, 10000);

    return () => clearInterval(interval);
  }, [schedules]);

  const handleConfirm = (date) => {
    let updatedSchedules = [...schedules];
    const isoTime = date.toISOString(); // ✅ Changed from toLocaleString() to toISOString()

    if (editIndex !== null) {
      updatedSchedules[editIndex] = isoTime;
      setEditIndex(null);
    } else {
      updatedSchedules.push(isoTime);
    }

    updatedSchedules.sort((a, b) => new Date(a) - new Date(b));
    setSchedules(updatedSchedules);
    updateSchedulesInFirebase(updatedSchedules);
    setShowPicker(false);
    setSelectedDate(null);
  };

  const updateSchedulesInFirebase = async (updatedSchedules) => {
    try {
      const schedulesRef = ref(db, "schedules");
      await set(schedulesRef, updatedSchedules);
    } catch (error) {
      console.error("Error updating schedules:", error);
    }
  };

  const handleDelete = (index) => {
    Alert.alert("Delete Schedule", "Are you sure you want to delete this schedule?", [
      { text: "Cancel", style: "cancel" },
      {
        text: "Delete",
        onPress: () => {
          const updatedSchedules = schedules.filter((_, i) => i !== index);
          setSchedules(updatedSchedules);
          updateSchedulesInFirebase(updatedSchedules);
        },
        style: "destructive",
      },
    ]);
  };

  const handleEdit = (index) => {
    setEditIndex(index);
    setSelectedDate(new Date(schedules[index])); // ✅ No change needed
    setShowPicker(true);
  };

  const addToHistory = (action) => {
    const historyRef = ref(db, `SmartFeeder/history/${Date.now()}`);
    set(historyRef, { type: action, timestamp: new Date().toLocaleString() });
  };

  const handleFeedFood = () => {
    set(ref(db, `SmartFeeder/foodcount`), true)
      .then(() => {
        alert(`Food Supplied!`);
        addToHistory("Food");
        setTimeout(() => {
          set(ref(db, `SmartFeeder/foodcount`), false);
        }, 10000);
      })
      .catch((error) => console.error(`Error setting food supply:`, error));
  };

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Feeding Schedule</Text>
      <TouchableOpacity
        style={styles.addButton}
        onPress={() => {
          setSelectedDate(null);
          setShowPicker(true);
        }}
      >
        <Ionicons name="add-circle" size={30} color="#fff" />
        <Text style={styles.addButtonText}>Add New Schedule</Text>
      </TouchableOpacity>

      <DateTimePickerModal
        isVisible={showPicker}
        mode="datetime"
        date={selectedDate || new Date()}
        onConfirm={handleConfirm}
        onCancel={() => setShowPicker(false)}
      />

      <FlatList
        data={schedules}
        keyExtractor={(item, index) => index.toString()}
        contentContainerStyle={styles.listContainer}
        renderItem={({ item, index }) => (
          <View style={styles.scheduleItem}>
            <Text style={styles.scheduleText}>
              {new Date(item).toLocaleString()} {/* ✅ Format ISO time to local string */}
            </Text>
            <View style={styles.iconContainer}>
              <TouchableOpacity onPress={() => handleEdit(index)}>
                <Ionicons name="pencil" size={24} color="#007bff" />
              </TouchableOpacity>
              <TouchableOpacity onPress={() => handleDelete(index)} style={{ marginLeft: 15 }}>
                <Ionicons name="trash" size={24} color="#d9534f" />
              </TouchableOpacity>
            </View>
          </View>
        )}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
    padding: 20,
    backgroundColor: "#f4f6f8",
  },
  title: {
    fontSize: 28,
    fontWeight: "bold",
    marginBottom: 20,
  },
  addButton: {
    backgroundColor: "#3CAD45",
    padding: 15,
    borderRadius: 10,
    marginBottom: 20,
    flexDirection: "row",
    alignItems: "center",
  },
  addButtonText: {
    color: "#fff",
    fontSize: 18,
    marginLeft: 10,
  },
  listContainer: {
    alignItems: "center",
    width: "100%",
  },
  scheduleItem: {
    backgroundColor: "#fff",
    padding: 15,
    marginVertical: 5,
    borderRadius: 8,
    flexDirection: "row",
    justifyContent: "space-between",
    alignItems: "center",
    width: "90%",
    elevation: 2,
  },
  scheduleText: {
    fontSize: 16,
  },
  iconContainer: {
    flexDirection: "row",
    alignItems: "center",
  },
});
