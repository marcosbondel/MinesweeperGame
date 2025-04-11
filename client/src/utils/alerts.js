import Swal from "sweetalert2";

const showSuccessAlert = (title, text) => {
    Swal.fire({
      title,
      text,
      icon: "success",
      confirmButtonText: "OK",
    });
};

const showWarningAlert = (title, text) => {
    Swal.fire({
        title,
        text,
        icon: "warning",
        showCancelButton: true,
        confirmButtonColor: "#ff9800",
        cancelButtonColor: "#757575",
        confirmButtonText: "Yes, do it!"
    }).then((result) => {
        if (result.isConfirmed) {
        Swal.fire("Deleted!", "Your file has been deleted.", "success");
        }
    });
};

const showErrorAlert = (title, text) => {
    Swal.fire({
        title,
        text,
        icon: "error",
        confirmButtonColor: "#d32f2f",
        confirmButtonText: "OK",
    });
};


export { 
    showSuccessAlert,
    showWarningAlert,
    showErrorAlert
};